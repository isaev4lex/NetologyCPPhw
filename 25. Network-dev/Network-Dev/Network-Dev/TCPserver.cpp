#include "TCPserver.h"

TCPserver::TCPserver() {
    _port_ = 2282;
    WSAStartup(MAKEWORD(2, 2), &_wsa_data_);

    _server_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    _server_address_.sin_family = AF_INET;
    _server_address_.sin_addr.s_addr = INADDR_ANY;
    _server_address_.sin_port = htons(_port_);

    bind(_server_socket_, (sockaddr*)&_server_address_, sizeof(_server_address_));
    listen(_server_socket_, SOMAXCONN);
}

void TCPserver::handle_client(SOCKET client_socket) {
    char buffer[1024] = { 0 };
    int received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (received > 0) {
        std::cout << "Received: " << buffer << std::endl;
        std::string reply = "Hello from server to client";
        send(client_socket, reply.c_str(), reply.size(), 0);
    }
    closesocket(client_socket);
}

void TCPserver::start() {
    std::cout << "Server is running and waiting for connections..." << std::endl;
    while (true) {
        SOCKET client_socket = accept(_server_socket_, NULL, NULL);
        std::thread t(&TCPserver::handle_client, this, client_socket);
        t.detach();
    }
}

TCPserver::~TCPserver() {
    closesocket(_server_socket_);
    WSACleanup();
}
