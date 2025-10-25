#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>
#include <QHostAddress>

#define BIND_PORT 12345
#define MSG_TEXT_PREFIX 0x01

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket();
    void ReadDatagram(QNetworkDatagram datagram);
    void SendDatagram(QByteArray data);
    void SendText(const QString& text);

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket* serviceUdpSocket = nullptr;

signals:
    void sig_sendTimeToGUI(QDateTime data);
    void sig_textReceived(QString text, QString sender, int sizeBytes);
};

#endif // UDPWORKER_H
