#include "tcpclient.h"




/* ServiceHeader
 * Для работы с потоками наши данные необходимо сериализовать.
 * Поскольку типы данных не стандартные перегрузим оператор << Для работы с ServiceHeader
*/
QDataStream &operator >>(QDataStream &out, ServiceHeader &data){

    out >> data.id;
    out >> data.idData;
    out >> data.status;
    out >> data.len;
    return out;
};
QDataStream &operator <<(QDataStream &in, ServiceHeader &data){

    in << data.id;
    in << data.idData;
    in << data.status;
    in << data.len;

    return in;
};



/*
 * Поскольку мы являемся клиентом, инициализацию сокета
 * проведем в конструкторе. Также необходимо соединить
 * сокет со всеми необходимыми нам сигналами.
*/
TCPclient::TCPclient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::readyRead,     this, &TCPclient::ReadyReed);
    connect(socket, &QTcpSocket::connected,     this, [this]{
        emit sig_connectStatus(STATUS_SUCCES);
    });
    connect(socket, &QTcpSocket::disconnected,  this, [this]{
        emit sig_Disconnected();
    });
    connect(socket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError){
        if (socket->state() != QAbstractSocket::ConnectedState) {
            emit sig_connectStatus(ERR_CONNECT_TO_HOST);
        }
    });
}

/* write
 * Метод отправляет запрос на сервер. Сериализировать будем
 * при помощи QDataStream
*/
void TCPclient::SendRequest(ServiceHeader head)
{
    QByteArray pkt;
    QDataStream out(&pkt, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);

    out << head;
    socket->write(pkt);
    socket->flush();
}

/* write
 * Такой же метод только передаем еще данные.
*/
void TCPclient::SendData(ServiceHeader head, QString str)
{
    // Сначала сериализуем полезную нагрузку отдельно,
    // чтобы корректно посчитать длину.
    QByteArray payload;
    {
        QDataStream pout(&payload, QIODevice::WriteOnly);
        pout.setVersion(QDataStream::Qt_6_5);
        pout << str;
    }

    head.len = payload.size();

    QByteArray pkt;
    QDataStream out(&pkt, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_5);
    out << head;
    pkt.append(payload);

    socket->write(pkt);
    socket->flush();
}

void TCPclient::ConnectToHost(QHostAddress host, uint16_t port)
{
    socket->connectToHost(host, port);
}

void TCPclient::DisconnectFromHost()
{
    if (socket->state() != QAbstractSocket::UnconnectedState)
        socket->disconnectFromHost();
}


void TCPclient::ReadyReed()
{

    QDataStream incStream(socket);
    incStream.setVersion(QDataStream::Qt_6_5);
    if(incStream.status() != QDataStream::Ok){
        QMessageBox msg;
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Ошибка открытия входящего потока для чтения данных!");
        msg.exec();
    }


    while (!incStream.atEnd()) {
        if(servHeader.idData == 0){

            if(socket->bytesAvailable() < sizeof(ServiceHeader)){
                return;
            }
            else{
                incStream >> servHeader;
                if(servHeader.id != ID){
                    uint16_t hdr = 0;
                    while (!incStream.atEnd()) {
                        incStream >> hdr;
                        if (hdr == ID) {
                            incStream >> servHeader.idData;
                            incStream >> servHeader.status;
                            incStream >> servHeader.len;
                            break;
                        }
                    }
                }
            }
        }
        if(socket->bytesAvailable() < servHeader.len){
            return;
        }
        else{
            ProcessingData(servHeader, incStream);
            servHeader.idData = 0;
            servHeader.status = 0;
            servHeader.len = 0;
        }
    }
}


void TCPclient::ProcessingData(ServiceHeader header, QDataStream &stream)
{
    stream.setVersion(QDataStream::Qt_6_5);

    if (header.status != STATUS_SUCCES) {
        emit sig_Error(header.status);
        return;
    }

    switch (header.idData) {

    case GET_TIME: {
        QDateTime dt;
        stream >> dt;
        emit sig_sendTime(dt);
        return;
    }

    case GET_SIZE: {
        quint32 freeSize = 0;
        stream >> freeSize;
        emit sig_sendFreeSize(static_cast<uint32_t>(freeSize));
        return;
    }

    case GET_STAT: {
        StatServer st;
        stream >> st.incBytes
            >> st.sendBytes
            >> st.revPck
            >> st.sendPck
            >> st.workTime
            >> st.clients;
        emit sig_sendStat(st);
        return;
    }

    case SET_DATA: {
        QString reply;
        stream >> reply;
        emit sig_SendReplyForSetData(reply);
        return;
    }

    case CLEAR_DATA: {
        emit sig_Success(CLEAR_DATA);
        return;
    }

    default:
        return;
    }
}
