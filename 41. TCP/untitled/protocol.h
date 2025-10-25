#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QtGlobal>
#include <QDateTime>
#include <QString>

// #pragma pack(push, 1)

struct MsgHeader {
    quint16 magic;
    quint16 msgId;
    quint8  status;
    quint32 dataSize;
};

enum : quint16 {
    MSG_TIME_REQ      = 100,
    MSG_FREE_REQ      = 101,
    MSG_STAT_REQ      = 102,
    MSG_PUT_STRING    = 200,
    MSG_FREE_SPACE    = 201
};

enum : quint8 {
    ERR_NO_SPACE      = 10,
    ERR_NO_LENGTH     = 11
};

struct ServerStat {
    quint32 rxBytes;
    quint32 txBytes;
    quint32 rxPackets;
    quint32 txPackets;
    quint32 uptimeSec;
    quint32 clients;
};

// #pragma pack(pop)

#endif // PROTOCOL_H
