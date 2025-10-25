#include "udpworker.h"
#include <QDataStream>

UDPworker::UDPworker(QObject *parent) : QObject(parent) {}

void UDPworker::InitSocket()
{
    serviceUdpSocket = new QUdpSocket(this);
    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);
    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);
}

void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{
    const QByteArray raw = datagram.data();

    if (!raw.isEmpty() && static_cast<unsigned char>(raw[0]) == MSG_TEXT_PREFIX) {
        QByteArray payload = raw.mid(1);
        QString text = QString::fromUtf8(payload);
        const QString sender = datagram.senderAddress().toString();
        emit sig_textReceived(text, sender, payload.size());
        return;
    }


    {
        QDataStream inStr(raw);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        inStr.setVersion(QDataStream::Qt_6_0);
#else
        inStr.setVersion(QDataStream::Qt_5_15);
#endif
        QDateTime dateTime;
        inStr >> dateTime;
        if (inStr.status() == QDataStream::Ok && dateTime.isValid()) {
            emit sig_sendTimeToGUI(dateTime);
            return;
        }
    }

    {
        QString text = QString::fromUtf8(raw);
        const QString sender = datagram.senderAddress().toString();
        emit sig_textReceived(text, sender, raw.size());
    }
}

void UDPworker::SendDatagram(QByteArray data)
{
    serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT);
}

void UDPworker::SendText(const QString& text)
{
    QByteArray payload = text.toUtf8();
    QByteArray packet;
    packet.reserve(1 + payload.size());
    packet.append(char(MSG_TEXT_PREFIX));
    packet.append(payload);
    serviceUdpSocket->writeDatagram(packet, QHostAddress::LocalHost, BIND_PORT);
}

void UDPworker::readPendingDatagrams()
{
    while (serviceUdpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
        ReadDatagram(datagram);
    }
}
