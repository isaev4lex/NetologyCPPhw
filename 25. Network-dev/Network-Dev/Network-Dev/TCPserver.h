#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "allinclude.h"

class TCPserver {
private:
    WSAData _wsa_data_;
    SOCKET _server_socket_;
    sockaddr_in _server_address_;
    int _port_;

    void handle_client(SOCKET client_socket);

public:
    TCPserver();
    void start();
    ~TCPserver();
};

#endif
