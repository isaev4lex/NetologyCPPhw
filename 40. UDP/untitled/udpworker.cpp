#include "udpworker.h"

UDPWorker::UDPWorker(QObject *parent, quint16 localPort, QHostAddress sendAddr, quint16 sendPort)
    : QObject(parent),
    m_sendAddr(sendAddr),
    m_sendPort(sendPort)
{
    bool ok = m_socket.bind(QHostAddress::LocalHost, localPort,
                            QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if (!ok) {
    }

    connect(&m_socket, &QUdpSocket::readyRead, this, &UDPWorker::onReadyRead);
}

void UDPWorker::send(const QString &text)
{
    if (text.isEmpty())
        return;

    QByteArray payload = text.toUtf8();
    m_socket.writeDatagram(payload, m_sendAddr, m_sendPort);
}

void UDPWorker::onReadyRead()
{
    while (m_socket.hasPendingDatagrams()) {
        QHostAddress senderAddr;
        quint16 senderPort = 0;
        QByteArray buffer;
        buffer.resize(int(m_socket.pendingDatagramSize()));

        const qint64 read = m_socket.readDatagram(buffer.data(), buffer.size(), &senderAddr, &senderPort);
        if (read >= 0) {
            const int sizeBytes = buffer.size();
            const QString line =
                QStringLiteral("Принято сообщение от %1:%2, размер сообщения(байт) %3")
                    .arg(senderAddr.toString())
                    .arg(senderPort)
                    .arg(sizeBytes);
            emit receivedLine(line);
        }
    }
}
