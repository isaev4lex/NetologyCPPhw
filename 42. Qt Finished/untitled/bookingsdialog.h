#ifndef BOOKINGSDIALOG_H
#define BOOKINGSDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
class DbManager;
namespace Ui { class BookingsDialog; }

class BookingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit BookingsDialog(DbManager* db, QWidget* parent = nullptr);
    ~BookingsDialog();
private slots:
    void onSelectionChanged();
    void onDeleteClicked();
private:
    Ui::BookingsDialog* ui;
    DbManager* m_db;
    QSqlQueryModel* m_model;
    int m_selectedBookingId = -1;
};
#endif
