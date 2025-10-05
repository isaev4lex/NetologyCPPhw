#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stopwatch.h"

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
    void onStartStop();
    void onClear();
    void onLap();

    void updateTimeLabel(qint64 ms);
    void onRunningChanged(bool running);
    void onLapRecorded(int index, qint64 lapMs);

private:
    static QString formatTenths(qint64 ms);

    Ui::MainWindow *ui;
    Stopwatch       sw_;
};

#endif // MAINWINDOW_H
