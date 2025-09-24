#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QTimer>
#include <QHBoxLayout>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartStop();
    void onReset();
    void onLap();
    void updateTime();

private:
    Ui::MainWindow *ui;
    QTimer timer_;
    QElapsedTimer baseTime_;
    int lapCount_ = 0;
    int startBtnHeight_ = 0;
    void applyBtnStyle(QPushButton* b, const QString& bg, const QString& fg);

    QWidget *lapContent_ = nullptr;
    QHBoxLayout *lapLayout_ = nullptr;

    QPushButton *resetButton_ = nullptr;
    QPushButton *lapButton_ = nullptr;

    void addLap(const QString &lapTime, const QString &totalTime);
    void clearLaps();
};
#endif // MAINWINDOW_H
