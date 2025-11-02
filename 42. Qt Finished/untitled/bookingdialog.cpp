#include "bookingdialog.h"
#include "ui_bookingdialog.h"
#include <QMessageBox>

BookingDialog::BookingDialog(const FlightInfo &info, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::BookingDialog)
{
    ui->setupUi(this);
    setWindowTitle("Оформление бронирования");

    ui->lbRoute->setText(QString("%1 (%2) → %3 (%4)")
                             .arg(info.fromName, info.fromCode,
                                  info.toName, info.toCode));
    auto fmt = [](const QString& iso) {
        QDateTime dt = QDateTime::fromString(iso, Qt::ISODate);
        dt.setTimeSpec(Qt::UTC);
        return QLocale().toString(dt.toLocalTime(), "dd.MM.yyyy  HH:mm");
    };
    ui->lbTimes->setText(QString("%1  →  %2").arg(fmt(info.depTime), fmt(info.arrTime)));
    ui->lbAirline->setText(info.airline.isEmpty() ? "Авиакомпания: —" : info.airline);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this]() {
        if (ui->leName->text().trimmed().isEmpty() || ui->lePassport->text().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Поля пусты", "Укажите ФИО и номер паспорта.");
            return;
        }
        accept();
    });
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

BookingDialog::~BookingDialog()
{
    delete ui;
}

QString BookingDialog::passengerName() const { return ui->leName->text().trimmed(); }
QString BookingDialog::passport() const      { return ui->lePassport->text().trimmed(); }
