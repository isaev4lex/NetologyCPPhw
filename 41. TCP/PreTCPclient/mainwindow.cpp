#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    client = new TCPclient(this);
    //Доступность полей по умолчанию
    ui->le_data->setEnabled(false);
    ui->pb_request->setEnabled(false);
    ui->lb_connectStatus->setText("Отключено");
    ui->lb_connectStatus->setStyleSheet("color: red");


    //При отключении меняем надписи и доступность полей.
    connect(client, &TCPclient::sig_Disconnected, this, [&]{
        ui->lb_connectStatus->setText("Отключено");
        ui->lb_connectStatus->setStyleSheet("color: red");
        ui->pb_connect->setText("Подключиться");
        ui->le_data->setEnabled(false);
        ui->pb_request->setEnabled(false);
        ui->spB_port->setEnabled(true);
        ui->spB_ip1->setEnabled(true);
        ui->spB_ip2->setEnabled(true);
        ui->spB_ip3->setEnabled(true);
        ui->spB_ip4->setEnabled(true);

    });


    connect(client, &TCPclient::sig_sendTime,        this, &MainWindow::DisplayTime);
    connect(client, &TCPclient::sig_sendFreeSize,    this, &MainWindow::DisplayFreeSpace);
    connect(client, &TCPclient::sig_sendStat,        this, &MainWindow::DisplayStat);
    connect(client, &TCPclient::sig_SendReplyForSetData, this, &MainWindow::SetDataReply);
    connect(client, &TCPclient::sig_Success,         this, &MainWindow::DisplaySuccess);
    connect(client, &TCPclient::sig_Error,           this, &MainWindow::DisplayError);
    connect(client, &TCPclient::sig_connectStatus,   this, &MainWindow::DisplayConnectStatus);

 /*
  * Соединяем сигналы со слотами
 */


}

MainWindow::~MainWindow()
{
    delete ui;
}

/*!
 * \brief Группа методо отображения различных данных
 */
void MainWindow::DisplayTime(QDateTime time)
{
    ui->tb_result->append("Текущее время на сервере: " +
                          time.toString("dd MMM yyyy HH:mm:ss"));
}

void MainWindow::DisplayFreeSpace(uint32_t freeSpace)
{
    ui->tb_result->append("Свободное место на сервере: " + QString::number(freeSpace) + " байт");
}

void MainWindow::SetDataReply(QString replyString)
{
    ui->tb_result->append("Сервер ответил на запись данных: " + replyString);
}

void MainWindow::DisplayStat(StatServer stat)
{
    ui->tb_result->append(
        "Статистика сервера:\n"
        "  Принято байт: " + QString::number(stat.incBytes) + "\n" +
        "  Передано байт: " + QString::number(stat.sendBytes) + "\n" +
        "  Принято пакетов: " + QString::number(stat.revPck) + "\n" +
        "  Передано пакетов: " + QString::number(stat.sendPck) + "\n" +
        "  Время работы (с): " + QString::number(stat.workTime) + "\n" +
        "  Клиентов: " + QString::number(stat.clients)
        );
}

void MainWindow::DisplayError(uint16_t error)
{
    switch (error) {
    case ERR_NO_FREE_SPACE:
        ui->tb_result->append("Ошибка: Недостаточно места на сервере");
        break;
    case ERR_NO_FUNCT:
        ui->tb_result->append("Ошибка: Функционал не реализован на сервере");
        break;
    case ERR_CONNECT_TO_HOST:
        ui->tb_result->append("Ошибка подключения (см. статус выше)");
        break;
    default:
        ui->tb_result->append("Ошибка: код " + QString::number(error));
        break;
    }
}

void MainWindow::DisplaySuccess(uint16_t typeMess)
{
    switch (typeMess) {
    case CLEAR_DATA:
        ui->tb_result->append("Память сервера успешно очищена");
        break;
    default:
        ui->tb_result->append("Успех: тип " + QString::number(typeMess));
        break;
    }
}

/*!
 * \brief Метод отображает статус подключения
 */
void MainWindow::DisplayConnectStatus(uint16_t status)
{

    if(status == ERR_CONNECT_TO_HOST){

        ui->tb_result->append("Ошибка подключения к порту: " + QString::number(ui->spB_port->value()));

    }
    else{
        ui->lb_connectStatus->setText("Подключено");
        ui->lb_connectStatus->setStyleSheet("color: green");
        ui->pb_connect->setText("Отключиться");
        ui->spB_port->setEnabled(false);
        ui->pb_request->setEnabled(true);
        ui->spB_ip1->setEnabled(false);
        ui->spB_ip2->setEnabled(false);
        ui->spB_ip3->setEnabled(false);
        ui->spB_ip4->setEnabled(false);
    }

}

/*!
 * \brief Обработчик кнопки подключения/отключения
 */
void MainWindow::on_pb_connect_clicked()
{
    if(ui->pb_connect->text() == "Подключиться"){

        uint16_t port = ui->spB_port->value();

        QString ip = QString("%1.%2.%3.%4")
                         .arg(ui->spB_ip4->value())
                         .arg(ui->spB_ip3->value())
                         .arg(ui->spB_ip2->value())
                         .arg(ui->spB_ip1->value());

        client->ConnectToHost(QHostAddress(ip), port);

    }
    else{

        client->DisconnectFromHost();
    }
}

/*
 * Для отправки сообщения согласно ПИВ необходимо
 * заполнить заголовок и передать его на сервер. В ответ
 * сервер вернет информацию в соответствии с типом сообщения
*/
void MainWindow::on_pb_request_clicked()
{
    ServiceHeader header;
    header.id = ID;
    header.status = STATUS_SUCCES;
    header.len = 0;

    switch (ui->cb_request->currentIndex()) {

    case 0:
        header.idData = GET_TIME;
        client->SendRequest(header);
        return;

    case 1:
        header.idData = GET_SIZE;
        client->SendRequest(header);
        return;

    case 2:
        header.idData = GET_STAT;
        client->SendRequest(header);
        return;

    case 3: {
        header.idData = SET_DATA;
        const QString data = ui->le_data->text();
        client->SendData(header, data);
        return;
    }

    case 4:
        header.idData = CLEAR_DATA;
        client->SendRequest(header);
        return;

    default:
        ui->tb_result->append("Такой запрос не реализован в текущей версии");
        return;
    }
}

/*!
 * \brief Обработчик изменения индекса запроса
 */
void MainWindow::on_cb_request_currentIndexChanged(int index)
{
    //Разблокируем поле отправления данных только когда выбрано "Отправить данные"
    if(ui->cb_request->currentIndex() == 3){
        ui->le_data->setEnabled(true);
    }
    else{
        ui->le_data->setEnabled(false);
    }
}

