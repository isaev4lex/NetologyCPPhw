#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QHeaderView>
#include <QTimer>
#include <QDesktopServices>
#include <QUrl>

#include "db.h"
#include "textnormalizer.h"

static QString err2s(const QSqlError& e) { return e.text().isEmpty() ? "Unknown SQL error" : e.text(); }

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    try { cfg_ = Config::load(QStringLiteral("config.ini")); }
    catch (const std::exception& e) {
        QMessageBox::critical(this, tr("Config error"), QString::fromUtf8(e.what()));
        QTimer::singleShot(0, this, [this]{ close(); });
        return;
    }

    initDb();

    resultsModel_ = new QStandardItemModel(this);
    resultsModel_->setHorizontalHeaderLabels({tr("Path"), tr("Title"), tr("Score")});
    ui->tableResults->setModel(resultsModel_);
    ui->tableResults->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableResults->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableResults->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableResults->horizontalHeader()->setStretchLastSection(true);

    connect(ui->tableResults, &QTableView::doubleClicked, this, [this](const QModelIndex& i){
        if (!i.isValid()) return;
        const QString p = resultsModel_->item(i.row(), 0)->text(); // колонка Path
        if (!p.isEmpty())
            QDesktopServices::openUrl(QUrl::fromLocalFile(p));
    });

    wordsModel_ = new QStandardItemModel(this);
    wordsModel_->setHorizontalHeaderLabels({tr("Token"), tr("Total Freq"), tr("Docs")});
    ui->tableWords->setModel(wordsModel_);
    ui->tableWords->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWords->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWords->horizontalHeader()->setStretchLastSection(true);

    connect(ui->btnSearch, &QPushButton::clicked, this, &MainWindow::onSearch);
    connect(ui->lineQuery, &QLineEdit::returnPressed, this, &MainWindow::onSearch);

    loadWordsTab();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::initDb() {
    QString err;
    if (!Db::open(cfg_.db, &err)) {
        QMessageBox::critical(this, tr("DB error"), err);
        QTimer::singleShot(0, this, [this]{ close(); });
        return;
    }
    if (!Db::ensureSchema(&err)) {
        QMessageBox::critical(this, tr("Schema error"), err);
        QTimer::singleShot(0, this, [this]{ close(); });
        return;
    }
}

void MainWindow::loadWordsTab() {
    wordsModel_->removeRows(0, wordsModel_->rowCount());
    QSqlQuery q(QSqlDatabase::database("pg"));
    const char* sql =
        "SELECT w.token, COALESCE(SUM(dw.freq),0) AS total_freq, COUNT(DISTINCT dw.doc_id) AS docs "
        "FROM words w "
        "LEFT JOIN doc_words dw ON dw.word_id = w.id "
        "GROUP BY w.id "
        "ORDER BY total_freq DESC NULLS LAST, w.token ASC "
        "LIMIT 500;";
    if (!q.exec(sql)) {
        QMessageBox::critical(this, tr("Query error"), err2s(q.lastError()));
        return;
    }
    while (q.next()) {
        QList<QStandardItem*> row;
        row << new QStandardItem(q.value(0).toString());
        row << new QStandardItem(QString::number(q.value(1).toLongLong()));
        row << new QStandardItem(QString::number(q.value(2).toLongLong()));
        wordsModel_->appendRow(row);
    }
}

void MainWindow::onSearch() {
    const QString raw = ui->lineQuery->text();
    if (raw.trimmed().isEmpty()) { resultsModel_->removeRows(0, resultsModel_->rowCount()); return; }
    const auto tokens = tokenize(raw, cfg_.indexer.minTokenLen, cfg_.indexer.maxTokenLen);
    if (tokens.isEmpty()) { QMessageBox::information(this, tr("Info"), tr("Нет валидных слов.")); return; }
    runSearch(tokens);
}

void MainWindow::runSearch(const QStringList& tokens) {
    resultsModel_->removeRows(0, resultsModel_->rowCount());

    QStringList ph; ph.reserve(tokens.size());
    for (int i=0;i<tokens.size();++i) ph << "?";

    const QString sql =
        "WITH q AS (SELECT id FROM words WHERE token IN (" + ph.join(",") + ")),"
                                                                            "dw AS (SELECT dw.doc_id, SUM(dw.freq) AS score, COUNT(DISTINCT dw.word_id) AS matched "
                                                                            "      FROM doc_words dw JOIN q ON q.id = dw.word_id GROUP BY dw.doc_id) "
                                                                            "SELECT d.path, d.title, dw.score "
                                                                            "FROM dw JOIN documents d ON d.id = dw.doc_id "
                                                                            "WHERE dw.matched = " + QString::number(tokens.size()) + " "
                                           "ORDER BY dw.score DESC, d.path "
                                           "LIMIT " + QString::number(cfg_.search.limit) + ";";

    QSqlQuery q(QSqlDatabase::database("pg"));
    q.prepare(sql);
    for (const auto& t : tokens) q.addBindValue(t);

    if (!q.exec()) {
        QMessageBox::critical(this, tr("Query error"), err2s(q.lastError()) + "\n\n" + sql);
        return;
    }
    while (q.next()) {
        QList<QStandardItem*> row;
        row << new QStandardItem(q.value(0).toString());
        row << new QStandardItem(q.value(1).toString());
        row << new QStandardItem(QString::number(q.value(2).toLongLong()));
        resultsModel_->appendRow(row);
    }
    if (resultsModel_->rowCount() == 0)
        QMessageBox::information(this, tr("Результат"), tr("Ничего не найдено."));
}
