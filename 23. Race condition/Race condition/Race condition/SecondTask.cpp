#include "SecondTask.h"


void CalculationImitation::clear_screen() {
    std::string os_name = []() {
#if defined(_WIN32)
        return "Windows";
#elif defined(__linux__)
        return "Linux";
#elif defined(__APPLE__)
        return "macOS";
#else
        return "Unknown";
#endif
        }();

    if (os_name == "Windows") {
        system("cls");
    }
    else {
        system("clear");
    }
}

void CalculationImitation::worker(ThreadData* data) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> jitter(-30, 30);  // разброс задержки ±30 мс, чтоб равноммерно не шли.

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i != progress_bar_width; i++) {
        data->progress = i;
        int sleep_time = delay_ms + jitter(gen);
        std::this_thread::sleep_for(std::chrono::milliseconds(std::max(10, sleep_time)));
    }
    auto end = std::chrono::high_resolution_clock::now();
    data->duration = std::chrono::duration<double>(end - start).count();
    data->done = true;
}

void CalculationImitation::printer_loop() {
    while (!all_done) {
        clear_screen();
        {
            std::lock_guard<std::mutex> lock(print_mutex);
            std::cout << "#\tid\tProgress Bar\t\t\tTime\n";
            for (const auto& d : thread_data) {
                std::cout << d->index << '\t' << d->id << "\t[";
                for (int i = 0; i < progress_bar_width; ++i) {
                    std::cout << (i < d->progress ? '#' : ' ');
                }
                std::cout << "]\t";
                if (d->done) {
                    std::cout << std::fixed << std::setprecision(4) << d->duration << "s";
                }
                std::cout << "\n";
            }
        }

        bool finished = true;
        for (const auto& d : thread_data)
            if (!d->done) finished = false;

        if (finished)
            all_done = true;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

CalculationImitation::CalculationImitation(int bar_width, int threads, int delay)
    : progress_bar_width(bar_width), num_threads(threads), delay_ms(delay)
{
    for (int i = 0; i < num_threads; ++i) {
        int tid = std::hash<std::thread::id>{}(std::this_thread::get_id()) + i * 111;
        thread_data.emplace_back(std::make_unique<ThreadData>(i, tid));
    }
}

void CalculationImitation::run() {
    std::vector<std::thread> workers;
    for (int i = 0; i < num_threads; ++i)
        workers.emplace_back(&CalculationImitation::worker, this, thread_data[i].get());

    std::thread printer(&CalculationImitation::printer_loop, this);

    for (auto& t : workers) t.join();
    printer.join();

    std::cout << "\nAll threads finished.\n";
}
