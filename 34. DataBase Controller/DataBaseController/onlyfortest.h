#ifndef ONLYFORTEST_H
#define ONLYFORTEST_H

#include <QDialog>

namespace Ui {
class ONLYforTest;
}

class ONLYforTest : public QDialog
{
    Q_OBJECT

public:
    explicit ONLYforTest(QWidget *parent = nullptr);
    ~ONLYforTest();

private:
    Ui::ONLYforTest *ui;
};

#endif // ONLYFORTEST_H
