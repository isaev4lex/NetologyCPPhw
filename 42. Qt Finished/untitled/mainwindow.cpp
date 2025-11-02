#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>
#include <QSqlQueryModel>
#include <QDate>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QItemSelectionModel>

#include "dbmanager.h"
#include "bookingdialog.h"
#include "bookingsdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_db(new DbManager(this)),
      m_flightsModel(new QSqlQueryModel(this))
{
    ui->setupUi(this);

    m_status = new QLabel("Отключено", this);
    m_status->setStyleSheet("color:#c00;");
    ui->statusbar->addPermanentWidget(m_status);

    ui->deDate->setMinimumDate(QDate(2016, 8, 15));
    ui->deDate->setMaximumDate(QDate(2017, 9, 14));
    ui->deDate->setDate(QDate(2016, 8, 15));

    ui->tvFlights->setModel(m_flightsModel);
    ui->tvFlights->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvFlights->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvFlights->horizontalHeader()->setStretchLastSection(true);
    ui->tvFlights->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvFlights->verticalHeader()->setVisible(false);

    ui->pbSearch->setEnabled(false);
    ui->pbBuy->setEnabled(false);
    ui->pbBookings->setEnabled(false);

    m_retryTimer = new QTimer(this);
    m_retryTimer->setInterval(5000);
    m_retryTimer->setSingleShot(true);
    connect(m_retryTimer, &QTimer::timeout, this, &MainWindow::loadAirports);

    // Сигналы
    connect(ui->pbSearch,   &QPushButton::clicked, this, &MainWindow::onSearch);
    connect(ui->pbBuy,      &QPushButton::clicked, this, &MainWindow::onBuy);
    connect(ui->pbBookings, &QPushButton::clicked, this, &MainWindow::onShowBookings);
    connect(ui->tvFlights->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &MainWindow::onSelectionChanged);

    loadAirports();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadAirports()
{
    ui->cbFrom->clear();
    ui->cbTo->clear();

    const auto list = m_db->airports();
    if (list.isEmpty()) {
        setConnected(false);
        QMessageBox::critical(this, "Подключение к БД",
                              "Не удалось получить список аэропортов.\n"
                              "Проверь подключение к БД и нажми ОК — повторим попытку.");
        if (!m_retryTimer->isActive()) m_retryTimer->start();
        return;
    }

    for (const auto &p : list) {
        const QString visible = QString("%1 — %2").arg(p.first, p.second);
        ui->cbFrom->addItem(visible, p.first);
        ui->cbTo->addItem(visible, p.first);
    }
    if (ui->cbFrom->count() >= 2) {
        ui->cbFrom->setCurrentIndex(0);
        ui->cbTo->setCurrentIndex(1);
    }

    setConnected(true);
    ui->pbSearch->setEnabled(true);
    ui->pbBookings->setEnabled(true);
    ui->pbBuy->setEnabled(false);
}

void MainWindow::setConnected(bool ok)
{
    if (ok) {
        m_status->setText("Подключено");
        m_status->setStyleSheet("color:#0a0;");
    } else {
        m_status->setText("Отключено");
        m_status->setStyleSheet("color:#c00;");
    }
}

void MainWindow::onSearch()
{
    const QString from = ui->cbFrom->currentData().toString();
    const QString to   = ui->cbTo->currentData().toString();
    const QDate   date = ui->deDate->date();

    if (from.isEmpty() || to.isEmpty() || from == to) {
        QMessageBox::warning(this, "Поиск", "Выберите разные аэропорты отправления и прибытия.");
        return;
    }

    m_db->searchFlights(from, to, date, m_flightsModel);

    if (m_flightsModel->columnCount() > 0) {
        ui->tvFlights->setColumnHidden(0, true);
    }

    while (m_flightsModel->canFetchMore()) m_flightsModel->fetchMore();

    if (m_flightsModel->rowCount() == 0) {
        QMessageBox::information(this, "Результаты",
                                 "Рейсы не найдены для выбранных параметров.");
    }
    m_selectedFlightId = -1;
    ui->pbBuy->setEnabled(false);
}

void MainWindow::onSelectionChanged()
{
    const auto sel = ui->tvFlights->selectionModel()->selectedRows();
    if (sel.isEmpty()) {
        m_selectedFlightId = -1;
        ui->pbBuy->setEnabled(false);
        return;
    }
    const QModelIndex idx = sel.first();
    m_selectedFlightId = m_flightsModel->data(m_flightsModel->index(idx.row(), 0)).toLongLong();
    ui->pbBuy->setEnabled(m_selectedFlightId > 0);
}

void MainWindow::onBuy()
{
    if (m_selectedFlightId <= 0) {
        QMessageBox::warning(this, "Покупка", "Не выбран корректный рейс.");
        return;
    }
    const FlightInfo info = m_db->flightInfo(m_selectedFlightId);
    if (!info.ok) {
        QMessageBox::critical(this, "Покупка",
                              "Не удалось получить данные рейса.\n"
                              "Проверьте соединение или попробуйте другой рейс.");
        return;
    }

    BookingDialog dlg(info, this);
    if (dlg.exec() == QDialog::Accepted) {
        if (m_db->addBooking(m_selectedFlightId, dlg.passengerName(), dlg.passport())) {
            QMessageBox::information(this, "Покупка", "Бронирование создано.");
        } else {
            QMessageBox::critical(this, "Покупка", "Не удалось создать бронирование.");
        }
    }
}

void MainWindow::onShowBookings()
{
    BookingsDialog dlg(m_db, this);
    dlg.exec();
}
