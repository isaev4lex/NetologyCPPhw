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
	std::cout << "Welcome to our service, we have " << clients_count.load(std::memory_order_relaxed) << " clients in queue." << std::endl;
}

void QueueImitation::clients() {
	while (!operationist_available.load(std::memory_order_acquire)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	while (clients_count.load(std::memory_order_acquire) < max_clients_count) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		int tmp_var = clients_count.fetch_add(1, std::memory_order_acq_rel) + 1;
		std::cout << std::endl;
		std::cout << "New client arrived, total clients: " << tmp_var << std::endl;
	}

	std::cout << "No more new clients, max reached." << std::endl;
}

void QueueImitation::operationist() {
	while (clients_count.load(std::memory_order_acquire) > 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		int tmp_var = clients_count.fetch_sub(1, std::memory_order_acq_rel);
		std::cout << std::endl;
		std::cout << "Client number " << tmp_var << " has been served, remaining clients: " << tmp_var - 1 << std::endl;
	}

	if (clients_count.load(std::memory_order_acquire) == 0) {
		operationist_finished.store(true, std::memory_order_release);
		operationist_available.store(false, std::memory_order_release);
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




