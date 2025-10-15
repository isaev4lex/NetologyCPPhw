#include "mainwindow.h"
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>
#include "./ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sb_initNum->setValue(200000);
    ui->rb_synchOff->setChecked(true);

   race1 = new Controller(&m);
   race2 = new Controller(&m);

   concurRace1 = new ExampleRace(&m);
   concurRace2 = new ExampleRace(&m);

    //Сигналы по завершению работы потоков
    connect(race1, &Controller::sig_WorkFinish, [&](){
        //отображаем только когда второй поток закончит работу
        if(countFinish == 0){
            countFinish++;
        }
        else{
            ui->te_debug->append("Искомое число равно: " + QString::number(number) + ", а должно быть " +
                                  QString::number(ui->sb_initNum->value()*2));
            ui->pb_start->setEnabled(true);
        }

    });

    connect(race2, &Controller::sig_WorkFinish, [&](){

        if(countFinish == 0){
            countFinish++;
        }
        else{
            ui->te_debug->append("Искомое число равно: " + QString::number(number) + ", а должно быть " +
                                 QString::number(ui->sb_initNum->value()*2));
            ui->pb_start->setEnabled(true);
        }

    });


    connect(concurRace1, &ExampleRace::sig_Finish, [&](){

        if(countFinish == 0){
            countFinish++;
        }
        else{
            ui->te_debug->append("Искомое число равно: " + QString::number(number) + ", а должно быть " +
                                 QString::number(ui->sb_initNum->value()*2));
            ui->pb_start->setEnabled(true);
        }
    });

    connect(concurRace2, &ExampleRace::sig_Finish, [&](){

        if(countFinish == 0){
            countFinish++;
        }
        else{
            ui->te_debug->append("Искомое число равно: " + QString::number(number) + ", а должно быть " +
                                 QString::number(ui->sb_initNum->value()*2));
            ui->pb_start->setEnabled(true);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete race1;
    delete race2;
    delete concurRace1;
    delete concurRace2;
}

//Метод запускает два потока
void MainWindow::StartRace(void)
{
    if (ui->rb_qtConcur->isChecked()) {
        ui->te_debug->append("QtConcurrent: concurRace1 → concurRace2 (последовательно)");

        const bool useMutex = ui->rb_mutexOn->isChecked();
        const int iters = ui->sb_initNum->value();

        auto *w1 = new QFutureWatcher<void>(this);
        auto *w2 = new QFutureWatcher<void>(this);

        connect(w1, &QFutureWatcher<void>::finished, this, [=]() {
            ui->te_debug->append("concurRace1 finished → запускаю concurRace2");
            w1->deleteLater();
            QFuture<void> f2 = QtConcurrent::run([=]() {
                concurRace2->DoWork(&number, useMutex, iters);
            });
            w2->setFuture(f2);
        });

        connect(w2, &QFutureWatcher<void>::finished, this, [=]() {
            ui->te_debug->append("concurRace2 finished");
            w2->deleteLater();
        });

        QFuture<void> f1 = QtConcurrent::run([=]() {
            concurRace1->DoWork(&number, useMutex, iters);
        });
        w1->setFuture(f1);
    } else {
        race1->operate(&number, ui->rb_mutexOn->isChecked(), ui->sb_initNum->value());
        race2->operate(&number, ui->rb_mutexOn->isChecked(), ui->sb_initNum->value());
    }
}

//Обработка кнопки "Старт"
void MainWindow::on_pb_start_clicked()
{
    ui->pb_start->setEnabled(false);
    countFinish = 0;
    number = 0;
    StartRace( );
}

