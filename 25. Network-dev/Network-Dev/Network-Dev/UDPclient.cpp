#include "UDPclient.h"


UDPclient::UDPclient() {
	_buffer_ = new char[32];
	_port_ = 2282;

	int error_code = WSAStartup(MAKEWORD(2, 2), &_wsa_data_);
	_socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	_server_address_.sin_family = AF_INET;
	std::string ip = "127.0.0.1";
	_server_address_.sin_addr.s_addr = inet_addr(ip.c_str());
	_server_address_.sin_port = htons(_port_);
}

void UDPclient::send_data(std::string message) {
	int server_size = sizeof(_server_address_);
	sendto(_socket_, message.c_str(), message.size(), 0, (const sockaddr*)&_server_address_, server_size);
	int size_data = recvfrom(_socket_, _buffer_, 32, 0, (sockaddr*)&_server_address_, &server_size);
	if (size_data > 0) {
		std::cout << _buffer_ << std::endl;
	}
}

UDPclient::~UDPclient() {
	delete[] _buffer_;
	closesocket(_socket_);
	WSACleanup();
}

