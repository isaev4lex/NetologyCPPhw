#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

class UDPWorker : public QObject
{
    Q_OBJECT
public:
    explicit UDPWorker(QObject *parent = nullptr,
                       quint16 localPort = 45454,
                       QHostAddress sendAddr = QHostAddress::LocalHost,
                       quint16 sendPort = 45454);

public slots:
    void send(const QString &text);

signals:
    void receivedLine(const QString &line);

private slots:
    void onReadyRead();

private:
    QUdpSocket m_socket;
    QHostAddress m_sendAddr;
    quint16 m_sendPort;
};

#endif // UDPWORKER_H
