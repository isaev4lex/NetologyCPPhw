#include "UDPserver.h"

UDPserver::UDPserver() {
	_buffer_ = new char[32];
	_port_ = 2282;

	int error_code = WSAStartup(MAKEWORD(2, 2), &_wsa_data_);
	_socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	_server_address_.sin_family = AF_INET;
	_server_address_.sin_addr.s_addr = INADDR_ANY;
	_server_address_.sin_port = htons(_port_);
	int error_socket = bind(_socket_, (sockaddr*)&_server_address_, sizeof(_server_address_));
	if (error_socket == SOCKET_ERROR) {
		std::cout << "Error initializing socket" << std::endl;
		closesocket(_socket_);
		WSACleanup();
	}
}

void UDPserver::recieve_and_answer() {
	int client_size = sizeof(_client_address_);
	while (true) {
		std::cout << "Waiting for client..." << std::endl;
		int size_data = recvfrom(_socket_, _buffer_, 32, 0, (sockaddr*)&_client_address_, &client_size);
		if (size_data > 0) {
			std::cout << _buffer_ << std::endl;
			std::string answer = "Hello from server to client";
			sendto(_socket_, answer.c_str(), answer.size(), 0, (const sockaddr*)&_client_address_, client_size);
		}
	}
}

UDPserver::~UDPserver() {
	delete[] _buffer_;
	closesocket(_socket_);
	WSACleanup();
}


