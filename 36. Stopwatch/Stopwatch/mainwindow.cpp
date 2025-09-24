#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startBtnHeight_ = ui->startStopButton->height();
    ui->startStopButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    ui->startStopButton->setMinimumHeight(startBtnHeight_);

    applyBtnStyle(ui->startStopButton, "#39a9ce", "black");

    connect(ui->startStopButton, &QPushButton::clicked, this, &MainWindow::onStartStop);
    connect(&timer_, &QTimer::timeout, this, &MainWindow::updateTime);
    timer_.setInterval(10);

    lapContent_ = new QWidget(ui->lapArea);
    lapLayout_  = new QHBoxLayout(lapContent_);
    lapLayout_->setContentsMargins(0,0,0,0);
    lapLayout_->setSpacing(8);
    lapLayout_->addStretch();

    ui->lapArea->setWidget(lapContent_);
    ui->lapArea->setWidgetResizable(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::applyBtnStyle(QPushButton* b, const QString& bg, const QString& fg)
{
    b->setStyleSheet(QString(
                         "QPushButton {"
                         "  background-color:%1;"
                         "  color:%2;"
                         "  border:1px solid #2b2b2b;"
                         "  border-radius:28px;"
                         "  padding:8px 16px;"
                         "}"
                         "QPushButton:pressed {"
                         "  filter:brightness(0.9);"
                         "}"
                         ).arg(bg, fg));
    b->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    b->setMinimumHeight(startBtnHeight_);
}

void MainWindow::onStartStop()
{
    if (!timer_.isActive()) {
        baseTime_.start();
        timer_.start();

        ui->startStopButton->setText("Stop");
        applyBtnStyle(ui->startStopButton, "red", "white");

        int idxStart = ui->verticalLayout->indexOf(ui->startStopButton);

        if (!resetButton_) {
            resetButton_ = new QPushButton("Reset", this);
            resetButton_->setFont(ui->startStopButton->font());
            applyBtnStyle(resetButton_, "#6b6f75", "white");
            resetButton_->setEnabled(true);
            ui->verticalLayout->insertWidget(idxStart + 1, resetButton_);
            connect(resetButton_, &QPushButton::clicked, this, &MainWindow::onReset);
        } else {
            applyBtnStyle(resetButton_, "#6b6f75", "white");
            resetButton_->show();
        }

        if (!lapButton_) {
            lapButton_ = new QPushButton("Lap", this);
            lapButton_->setFont(ui->startStopButton->font());
            applyBtnStyle(lapButton_, "#6b6f75", "white");
            ui->verticalLayout->insertWidget(idxStart + 2, lapButton_);
            connect(lapButton_, &QPushButton::clicked, this, &MainWindow::onLap);
        } else {
            applyBtnStyle(lapButton_, "#6b6f75", "white");
            lapButton_->show();
        }

    } else {
        timer_.stop();

        ui->startStopButton->setText("Start");
        applyBtnStyle(ui->startStopButton, "#39a9ce", "black");

        if (lapButton_) lapButton_->hide();
        if (resetButton_) resetButton_->show();
    }
}

void MainWindow::onReset()
{
    timer_.stop();
    ui->leStopwatch->setText("00:00.00");
    clearLaps();
    lapCount_ = 0;

    if (resetButton_) resetButton_->hide();
    if (lapButton_)  lapButton_->hide();

    ui->startStopButton->setText("Start");
    applyBtnStyle(ui->startStopButton, "#39a9ce", "black");
}

void MainWindow::onLap()
{
    lapCount_++;
    QString total = ui->leStopwatch->text();
    QString lapTime = total;

    addLap(lapTime, total);
}

void MainWindow::updateTime()
{
    int ms = baseTime_.elapsed();
    int sec = ms / 1000;
    int min = sec / 60;

    QString text = QString("%1:%2.%3")
                       .arg(min, 2, 10, QLatin1Char('0'))
                       .arg(sec % 60, 2, 10, QLatin1Char('0'))
                       .arg((ms % 1000) / 10, 2, 10, QLatin1Char('0'));

    ui->leStopwatch->setText(text);
}

void MainWindow::addLap(const QString& lapTime, const QString& totalTime)
{
    QWidget* box = new QWidget(lapContent_);
    QVBoxLayout* lay = new QVBoxLayout(box);
    lay->setContentsMargins(6,6,6,6);

    QLabel* num = new QLabel(QString("Lap %1").arg(lapCount_), box);
    QLabel* l1 = new QLabel("Lap: " + lapTime, box);
    QLabel* l2 = new QLabel("Total: " + totalTime, box);

    num->setAlignment(Qt::AlignCenter);
    l1->setAlignment(Qt::AlignCenter);
    l2->setAlignment(Qt::AlignCenter);

    lay->addWidget(num);
    lay->addWidget(l1);
    lay->addWidget(l2);

    box->setFixedSize(90, 70);
    box->setStyleSheet("background:#2b2d31; color:white; border-radius:8px;");

    lapLayout_->insertWidget(lapLayout_->count()-1, box);

    ui->lapArea->horizontalScrollBar()->setValue(ui->lapArea->horizontalScrollBar()->maximum());
}

void MainWindow::clearLaps()
{
    QLayoutItem* item;
    while ((item = lapLayout_->takeAt(0))) {
        if (item->widget()) delete item->widget();
        delete item;
    }
    lapLayout_->addStretch();
}
