#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QHeaderView>

namespace {
// ДАННЫЕ ИЗ ЗАДАНИЯ
constexpr int DB_PORT = 5432;
const char* DB_HOST = "981757-ca08998.tmweb.ru";
const char* DB_NAME = "netology_cpp";
const char* DB_USER = "netology_usr_cpp";
const char* DB_PASS = "CppNeto3";
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnReload, &QPushButton::clicked, this, &MainWindow::onReload);
    connect(ui->btnClear,  &QPushButton::clicked, this, &MainWindow::onClear);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->setVisible(false);
    if (!ensureConnection()) {
        setEmptyModel();
        return;
    }
    onReload();
}

MainWindow::~MainWindow()
{
    delete m_tableModel;
    delete m_queryModel;

    if (m_db.isValid() && m_db.isOpen()) {
        m_db.close();
    }
    delete ui;
}

bool MainWindow::ensureConnection()
{
    if (m_db.isValid() && m_db.isOpen())
        return true;

    const QString connName = QStringLiteral("pg_conn_main");
    if (QSqlDatabase::contains(connName)) {
        m_db = QSqlDatabase::database(connName);
    } else {
        m_db = QSqlDatabase::addDatabase(QStringLiteral("QPSQL"), connName);
        m_db.setHostName(QString::fromLatin1(DB_HOST));
        m_db.setPort(DB_PORT);
        m_db.setDatabaseName(QString::fromLatin1(DB_NAME));
        m_db.setUserName(QString::fromLatin1(DB_USER));
        m_db.setPassword(QString::fromLatin1(DB_PASS));
    }

    if (!m_db.open()) {
        QMessageBox::critical(this, tr("Ошибка подключения"),
                              tr("Не удалось подключиться к БД (%1)").arg(m_db.lastError().text()));
        return false;
    }
    return true;
}

void MainWindow::onReload()
{
    if (!ensureConnection()) {
        setEmptyModel();
        return;
    }

    const QString sel = ui->comboFilter->currentText().trimmed();
    if (sel.compare(QStringLiteral("все"), Qt::CaseInsensitive) == 0) {
        showAllWithTableModel();
    } else if (sel == QStringLiteral("Comedy") || sel == QStringLiteral("Horror")) {
        showCategory(sel);
    } else {
        showAllWithTableModel();
    }
}

void MainWindow::onClear()
{
    setEmptyModel();
}

void MainWindow::showAllWithTableModel()
{
    if (m_queryModel) {
        delete m_queryModel; m_queryModel = nullptr;
    }
    if (!m_tableModel) {
        m_tableModel = new QSqlTableModel(this, m_db);
        m_tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    }

    m_tableModel->setTable(QStringLiteral("film"));
    if (!m_tableModel->select()) {
        QMessageBox::warning(this, tr("Ошибка запроса"),
                             tr("Не удалось выбрать данные из film: %1").arg(m_tableModel->lastError().text()));
    }

    const int colTitle = m_tableModel->fieldIndex("title");
    const int colDescr = m_tableModel->fieldIndex("description");
    if (colTitle >= 0) m_tableModel->setHeaderData(colTitle, Qt::Horizontal, tr("Название фильма"));
    if (colDescr >= 0) m_tableModel->setHeaderData(colDescr, Qt::Horizontal, tr("Описание фильма"));

    ui->tableView->setModel(m_tableModel);
    applyTwoColumnsOnly();
}

void MainWindow::showCategory(const QString& name)
{
    if (m_tableModel) {
        delete m_tableModel; m_tableModel = nullptr;
    }
    if (!m_queryModel) {
        m_queryModel = new QSqlQueryModel(this);
    }

    const QString sql = QStringLiteral(
        "SELECT f.title AS \"Название фильма\", f.description AS \"Описание фильма\" "
        "FROM film f "
        "JOIN film_category fc ON f.film_id = fc.film_id "
        "JOIN category c ON c.category_id = fc.category_id "
        "WHERE c.name = :cat"
        );

    QSqlQuery q(m_db);
    if (!q.prepare(sql)) {
        QMessageBox::warning(this, tr("Ошибка запроса"),
                             tr("Подготовка запроса не удалась: %1").arg(q.lastError().text()));
        setEmptyModel();
        return;
    }
    q.bindValue(":cat", name);
    if (!q.exec()) {
        QMessageBox::warning(this, tr("Ошибка запроса"),
                             tr("Выполнение запроса не удалось: %1").arg(q.lastError().text()));
        setEmptyModel();
        return;
    }

    m_queryModel->setQuery(q);
    if (m_queryModel->lastError().isValid()) {
        QMessageBox::warning(this, tr("Ошибка запроса"),
                             tr("Модель вернула ошибку: %1").arg(m_queryModel->lastError().text()));
        setEmptyModel();
        return;
    }

    ui->tableView->setModel(m_queryModel);

    m_queryModel->setHeaderData(0, Qt::Horizontal, tr("Название фильма"));
    m_queryModel->setHeaderData(1, Qt::Horizontal, tr("Описание фильма"));
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::applyTwoColumnsOnly()
{
    auto *model = ui->tableView->model();
    if (!model) return;

    int titleCol = -1;
    int descrCol = -1;

    if (auto tm = qobject_cast<QSqlTableModel*>(model)) {
        titleCol = tm->fieldIndex("title");
        descrCol = tm->fieldIndex("description");
    } else {
        ui->tableView->resizeColumnsToContents();
        return;
    }

    const int cols = model->columnCount();
    for (int c = 0; c < cols; ++c) {
        bool show = (c == titleCol) || (c == descrCol);
        ui->tableView->setColumnHidden(c, !show);
    }

    ui->tableView->resizeColumnsToContents();
}

void MainWindow::setEmptyModel()
{
    ui->tableView->setModel(nullptr);
    if (m_tableModel) { delete m_tableModel; m_tableModel = nullptr; }
    if (m_queryModel) { delete m_queryModel; m_queryModel = nullptr; }
}
