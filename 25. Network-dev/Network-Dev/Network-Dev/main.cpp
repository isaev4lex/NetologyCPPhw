#include "allinclude.h"
#include "UDPserver.h"
#include "UDPclient.h"
#include "TCPserver.h"
#include "TCPclient.h"

int main() {

	/*UDPserver server;
	server.recieve_and_answer();

	UDPclient client;
	client.send_data("Hello from client to server");*/

	std::thread server_thread([]() {
		TCPserver server;
		server.start();
	});

	std::this_thread::sleep_for(std::chrono::seconds(1));

	TCPclient client;
	client.send_message("Hello from client to server");

	server_thread.join();

	return 0;
}