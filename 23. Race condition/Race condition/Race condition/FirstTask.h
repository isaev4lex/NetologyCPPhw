#pragma once
#include "allinclude.h"


class QueueImitation {
private:
    std::atomic<int> clients_count;
    std::atomic<int> clients_count1;
    int max_clients_count;
    std::atomic<bool> operationist_available;
    std::atomic<bool> operationist_finished;

    std::thread operationist_thread;
    std::thread clients_thread;

public:
    QueueImitation(int first_clients_count);

    void clients();
    void operationist();
    void start();

    ~QueueImitation();
};
