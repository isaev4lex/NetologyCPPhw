#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "newconnectiondiaglog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(this->size());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tbAdd_clicked()
{
    NewConnectionDiaglog dlg(this);
    dlg.exec();   // открывает как модальное окно
}

void MainWindow::on_tbAdd_2_clicked()
{
    NewConnectionDiaglog dlg(this);
    dlg.exec();
}
