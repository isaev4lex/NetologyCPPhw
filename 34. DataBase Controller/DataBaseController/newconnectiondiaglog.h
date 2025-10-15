#ifndef NEWCONNECTIONDIAGLOG_H
#define NEWCONNECTIONDIAGLOG_H

#include <QDialog>

namespace Ui {
class NewConnectionDiaglog;
}

class NewConnectionDiaglog : public QDialog
{
    Q_OBJECT

public:
    explicit NewConnectionDiaglog(QWidget *parent = nullptr);
    ~NewConnectionDiaglog();

private:
    Ui::NewConnectionDiaglog *ui;
};

#endif // NEWCONNECTIONDIAGLOG_H
