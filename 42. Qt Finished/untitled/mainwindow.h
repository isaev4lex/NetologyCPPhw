#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include <QLabel>
#include <QTimer>


class DbManager;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSearch();
    void onSelectionChanged();
    void onBuy();
    void onShowBookings();

private:
    void loadAirports();
    void setConnected(bool ok);

    Ui::MainWindow *ui;
    DbManager      *m_db;
    QSqlQueryModel *m_flightsModel;
    qint64 m_selectedFlightId = -1;

    QLabel        *m_status = nullptr;
    QTimer        *m_retryTimer = nullptr;
};

#endif // MAINWINDOW_H
