#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->radioButton->setText("Вариант A");
    ui->radioButton_2->setText("Вариант B");

    QStringList items{
        "PostgreSQL","MySQL","SQLite","Oracle","SQL Server",
        "MariaDB","Firebird","MongoDB","Redis","ClickHouse"
    };
    items = items.mid(0, qMin(items.size(), 10));
    ui->comboBox->clear();
    ui->comboBox->addItems(items);

    ui->pushButton->setText("Increment");
    ui->pushButton->setCheckable(true);

    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);

    connect(ui->pushButton, &QPushButton::clicked,
            this, &MainWindow::onIncClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onIncClicked()
{
    int v = ui->progressBar->value() + 10;
    if (v > ui->progressBar->maximum()) v = 0;
    ui->progressBar->setValue(v);
}
