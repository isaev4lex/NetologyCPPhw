#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    udpWorker = new UDPworker(this);
    udpWorker->InitSocket();

    connect(udpWorker, &UDPworker::sig_sendTimeToGUI, this, &MainWindow::DisplayTime);
    connect(udpWorker, &UDPworker::sig_textReceived,  this, &MainWindow::onTextDatagram);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{
        QDateTime dateTime = QDateTime::currentDateTime();
        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        outStr.setVersion(QDataStream::Qt_6_0);
#else
        outStr.setVersion(QDataStream::Qt_5_15);
#endif
        outStr << dateTime;
        udpWorker->SendDatagram(dataToSend);
    });


    connect(ui->pb_send, &QPushButton::clicked, this, &MainWindow::on_pb_send_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pb_start_clicked()
{
    timer->start(TIMER_DELAY);
}

void MainWindow::DisplayTime(QDateTime data)
{
    counterPck++;
    if (counterPck % 20 == 0) {
        ui->te_result->clear();
    }
    ui->te_result->append("Текущее время: " + data.toString()
                          + ". Принято пакетов " + QString::number(counterPck));
}

void MainWindow::on_pb_stop_clicked()
{
    timer->stop();
}

void MainWindow::on_pb_send_clicked()
{
    const QString text = ui->le_text->text();
    if (text.isEmpty()) return;
    udpWorker->SendText(text);
}

void MainWindow::onTextDatagram(QString text, QString sender, int sizeBytes)
{
    ui->te_result->append(QString("Принято сообщение от %1, размер сообщения(байт) %2\nТекст: %3")
                              .arg(sender).arg(sizeBytes).arg(text));
}
