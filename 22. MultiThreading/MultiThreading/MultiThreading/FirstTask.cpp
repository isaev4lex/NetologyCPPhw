#include "FirstTask.h"


QueueImitation::QueueImitation(int first_clients_count)
	: clients_count(first_clients_count),
	max_clients_count(30),
	operationist_available(true),
	operationist_finished(false) {

	if (first_clients_count > max_clients_count) {
		throw std::invalid_argument("Initial clients count exceeds maximum allowed clients count.");
	}
	else if (first_clients_count < 0) {
		throw std::invalid_argument("Initial clients count cannot be negative.");
	}
	std::cout << std::endl;
	std::cout << "Welcome to our service, we have " << clients_count << " clients in queue." << std::endl;
}

void QueueImitation::clients() {
	while (!operationist_available) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	while (clients_count < max_clients_count) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		clients_count++;
		std::cout << std::endl;
		std::cout << "New client arrived, total clients: " << clients_count << std::endl;
	}

	std::cout << "No more new clients, max reached." << std::endl;
}

void QueueImitation::operationist() {
	while (clients_count > 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		clients_count--;
		std::cout << std::endl;
		std::cout << "Client number " << clients_count + 1 << " has been served, remaining clients: " << clients_count << std::endl;
	}

	if (clients_count == 0) {
		operationist_finished = true;
		operationist_available = false;
		std::cout << std::endl;
		std::cout << "We are closing for today, come tomorrow." << std::endl;
	}
}

void QueueImitation::start() {
	operationist_thread = std::thread(&QueueImitation::operationist, this);
	clients_thread = std::thread(&QueueImitation::clients, this);
}

QueueImitation::~QueueImitation() {
	if (operationist_thread.joinable()) {
		operationist_thread.join();
	}

	if (clients_thread.joinable()) {
		clients_thread.join();
	}
}

