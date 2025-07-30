#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "allinclude.h"

class TCPclient {
private:
    WSAData _wsa_data_;
    SOCKET _socket_;
    sockaddr_in _server_address_;
    int _port_;

public:
    TCPclient();
    void send_message(const std::string& message);
    ~TCPclient();
};

#endif
