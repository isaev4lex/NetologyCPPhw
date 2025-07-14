#pragma once
#include "allinclude.h"


struct ThreadData {
    int index = 0;
    int id = 0;
    std::atomic<int> progress{ 0 };
    std::atomic<bool> done{ false };
    double duration = 0.0;

    ThreadData(int idx, int tid) : index(idx), id(tid) {}
    ThreadData(const ThreadData&) = delete;
    ThreadData& operator=(const ThreadData&) = delete;
    ThreadData(ThreadData&&) = delete;
    ThreadData& operator=(ThreadData&&) = delete;
};


class CalculationImitation {
private:
    std::mutex print_mutex;
    std::atomic<bool> all_done{ false };
    const int progress_bar_width;
    const int num_threads;
    const int delay_ms;

    std::vector<std::unique_ptr<ThreadData>> thread_data;

    void clear_screen();

    void worker(ThreadData* data);

    void printer_loop();

public:
    CalculationImitation(int bar_width, int threads, int delay);

    void run();
};
