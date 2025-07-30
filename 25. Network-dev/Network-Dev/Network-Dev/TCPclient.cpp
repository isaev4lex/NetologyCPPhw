#include "TCPclient.h"

TCPclient::TCPclient() {
    _port_ = 2282;
    WSAStartup(MAKEWORD(2, 2), &_wsa_data_);

    _socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    _server_address_.sin_family = AF_INET;
    _server_address_.sin_port = htons(_port_);
    _server_address_.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(_socket_, (sockaddr*)&_server_address_, sizeof(_server_address_));
}

void TCPclient::send_message(const std::string& message) {
    send(_socket_, message.c_str(), message.size(), 0);

    char buffer[1024] = { 0 };
    int received = recv(_socket_, buffer, sizeof(buffer) - 1, 0);
    if (received > 0) {
        std::cout << "Server says: " << buffer << std::endl;
    }
}

TCPclient::~TCPclient() {
    closesocket(_socket_);
    WSACleanup();
}
