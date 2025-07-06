#pragma once
#include "allinclude.h"


class QueueImitation {
private:
    int clients_count;
    int max_clients_count;
    bool operationist_available;
    bool operationist_finished;

    std::thread operationist_thread;
    std::thread clients_thread;

public:
    QueueImitation(int first_clients_count);

    void clients();
    void operationist();
    void start();

    ~QueueImitation();
};
