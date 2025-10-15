#include "onlyfortest.h"
#include "ui_onlyfortest.h"

ONLYforTest::ONLYforTest(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ONLYforTest)
{
    ui->setupUi(this);
}

ONLYforTest::~ONLYforTest()
{
    delete ui;
}
