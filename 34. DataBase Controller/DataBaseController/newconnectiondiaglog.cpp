#include "newconnectiondiaglog.h"
#include "ui_newconnectiondiaglog.h"

NewConnectionDiaglog::NewConnectionDiaglog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewConnectionDiaglog)
{
    ui->setupUi(this);
    setFixedSize(this->size());
}

NewConnectionDiaglog::~NewConnectionDiaglog()
{
    delete ui;
}
