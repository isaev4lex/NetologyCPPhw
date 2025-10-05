#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QString>

static inline QString two(int v){ return (v < 10 ? QString("0%1").arg(v) : QString::number(v)); }

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->labelTime->setText("00:00.0");
    ui->startStopButton->setText("Start");
    ui->lapButton->setEnabled(false);

    connect(ui->startStopButton, &QPushButton::clicked, this, &MainWindow::onStartStop);
    connect(ui->clearButton,     &QPushButton::clicked, this, &MainWindow::onClear);
    connect(ui->lapButton,       &QPushButton::clicked, this, &MainWindow::onLap);

    connect(&sw_, &Stopwatch::elapsedChanged, this, &MainWindow::updateTimeLabel);
    connect(&sw_, &Stopwatch::runningChanged, this, &MainWindow::onRunningChanged);
    connect(&sw_, &Stopwatch::lapRecorded,    this, &MainWindow::onLapRecorded);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onStartStop()
{
    if (sw_.isRunning()) {
        sw_.stop();
    } else {
        sw_.start();
    }
}

void MainWindow::onClear()
{
    sw_.reset();
    ui->lapsBrowser->clear();
}

void MainWindow::onLap()
{
    sw_.markLap();
}

void MainWindow::updateTimeLabel(qint64 ms)
{
    ui->labelTime->setText(formatTenths(ms));
}

void MainWindow::onRunningChanged(bool running)
{
    ui->startStopButton->setText(running ? "STOP!!!!" : "Start");
    ui->lapButton->setEnabled(running);
}

void MainWindow::onLapRecorded(int index, qint64 lapMs)
{
    const double secs = lapMs / 1000.0;
    ui->lapsBrowser->append(
        QString("Loop %1, time: %2 sec").arg(index).arg(QString::number(secs, 'f', 1))
        );
}

QString MainWindow::formatTenths(qint64 ms)
{
    if (ms < 0) ms = 0;
    const int totalSec = static_cast<int>(ms / 1000);
    const int min  = totalSec / 60;
    const int sec  = totalSec % 60;
    const int tenth = static_cast<int>((ms % 1000) / 100);
    return QString("%1:%2.%3").arg(two(min)).arg(two(sec)).arg(tenth);
}
