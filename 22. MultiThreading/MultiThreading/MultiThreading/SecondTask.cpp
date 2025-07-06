#include "SecondTask.h"


void VectorAdder::run() {
    std::cout << "Hardware threads available: " << std::thread::hardware_concurrency() << "\n\n";

    std::vector<int> thread_counts = { 2, 4, 8, 16 };
    std::vector<int> sizes = { 1000, 10000, 100000, 1000000 };

    std::cout << std::left << std::setw(12) << "Size"
        << std::setw(10) << "Threads"
        << std::setw(15) << "Time (ms)" << "\n";
    std::cout << "--------------------------------------\n";

    for (int size : sizes) {
        for (int num_threads : thread_counts) {
            double time = measure_sum(size, num_threads);
            std::cout << std::left << std::setw(12) << size
                << std::setw(10) << num_threads
                << std::setw(15) << std::fixed << std::setprecision(4) << time
                << "\n";
        }
    }
}

double VectorAdder::measure_sum(int size, int thread_count) {
    std::vector<int> A(size), B(size), C(size);

    for (int i = 0; i < size; ++i) {
        A[i] = i;
        B[i] = size - i;
    }

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    int chunk = size / thread_count;

    for (int t = 0; t < thread_count; ++t) {
        int start_idx = t * chunk;
        int end_idx = (t == thread_count - 1) ? size : start_idx + chunk;

        threads.emplace_back([&, start_idx, end_idx]() {
            for (int i = start_idx; i < end_idx; ++i) {
                C[i] = A[i] + B[i];
            }
            });
    }

    for (auto& th : threads) {
        th.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> diff = end - start;
    return diff.count();
}
