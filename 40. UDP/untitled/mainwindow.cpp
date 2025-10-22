#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "udpworker.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_udp(nullptr)
{
    ui->setupUi(this);

    m_udp = new UDPWorker(this, /*localPort*/45454, QHostAddress::LocalHost, /*sendPort*/45454);

    connect(ui->btnSend, &QPushButton::clicked, this, [this](){
        const QString text = ui->lineEditMessage->text();
        if (text.isEmpty())
            return;
        m_udp->send(text);
    });

    connect(m_udp, &UDPWorker::receivedLine, this, [this](const QString &line){
        ui->listReceived->addItem(line);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
