#ifndef BOOKINGDIALOG_H
#define BOOKINGDIALOG_H

#include <QDialog>
#include "dbmanager.h"

namespace Ui { class BookingDialog; }

class BookingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BookingDialog(const FlightInfo& info, QWidget *parent = nullptr);
    ~BookingDialog();

    QString passengerName() const;
    QString passport() const;

private:
    Ui::BookingDialog *ui;
};

#endif // BOOKINGDIALOG_H
