#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "allinclude.h"

class UDPserver {
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
	UDPserver();

	void recieve_and_answer();

	~UDPserver();
};


#endif