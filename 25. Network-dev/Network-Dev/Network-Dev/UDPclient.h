#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include "allinclude.h"


class UDPclient {
private:
	WSAData _wsa_data_;
	SOCKET _socket_;
	sockaddr_in _server_address_{

	};
	sockaddr_in _client_address_{

	};
	char* _buffer_;
	int _port_;

public:
	UDPclient();

	void send_data(std::string message);

	~UDPclient();
};

#endif