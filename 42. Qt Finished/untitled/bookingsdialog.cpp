#include "bookingsdialog.h"
#include "ui_bookingsdialog.h"
#include "dbmanager.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QAbstractItemView>

BookingsDialog::BookingsDialog(DbManager* db, QWidget* parent)
    : QDialog(parent), ui(new Ui::BookingsDialog), m_db(db), m_model(new QSqlQueryModel(this)) {
    ui->setupUi(this);
    ui->tvBookings->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvBookings->setAlternatingRowColors(true);
    ui->tvBookings->verticalHeader()->setVisible(false);

    setWindowTitle("Мои бронирования");

    ui->tvBookings->setModel(m_model);
    ui->tvBookings->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvBookings->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tvBookings->horizontalHeader()->setStretchLastSection(true);

    connect(ui->tvBookings->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &BookingsDialog::onSelectionChanged);
    connect(ui->pbDelete, &QPushButton::clicked, this, &BookingsDialog::onDeleteClicked);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    m_db->loadBookings(m_model);

    connect(m_model, &QAbstractItemModel::modelReset, this, [this]{
        if (m_model->columnCount() > 0) ui->tvBookings->setColumnHidden(0, true);
        m_selectedBookingId = -1;
        ui->pbDelete->setEnabled(false);
    });

    if (m_model->columnCount() > 0) ui->tvBookings->setColumnHidden(0, true);
    ui->pbDelete->setEnabled(false);
}

BookingsDialog::~BookingsDialog() { delete ui; }

void BookingsDialog::onSelectionChanged() {
    const auto sel = ui->tvBookings->selectionModel()->selectedRows();
    if (sel.isEmpty()) {
        m_selectedBookingId = -1;
        ui->pbDelete->setEnabled(false);
        return;
    }
    m_selectedBookingId = m_model->data(m_model->index(sel.first().row(), 0)).toInt();
    ui->pbDelete->setEnabled(m_selectedBookingId > 0);
}

void BookingsDialog::onDeleteClicked() {
    if (m_selectedBookingId <= 0) return;
    if (QMessageBox::question(this, "Удаление", "Удалить выбранное бронирование?") == QMessageBox::Yes) {
        if (m_db->deleteBooking(m_selectedBookingId)) {
            m_db->loadBookings(m_model);
            if (m_model->columnCount() > 0) ui->tvBookings->setColumnHidden(0, true);
            ui->pbDelete->setEnabled(false);
            m_selectedBookingId = -1;
            ui->tvBookings->clearSelection();
        } else {
            QMessageBox::critical(this, "Ошибка", "Не удалось удалить бронирование.");
        }
    }
}
