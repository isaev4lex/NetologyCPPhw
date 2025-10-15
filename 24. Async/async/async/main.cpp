#include <iostream>
#include <vector>
#include <future>
#include <thread>
#include <chrono>


namespace first_task {
    void find_min_index(const std::vector<int>& arr, int start, std::promise<int> prom) {
        int min_index = start;
        for (int i = start + 1; i < arr.size(); ++i) {
            if (arr[i] < arr[min_index]) {
                min_index = i;
            }
        }
        prom.set_value(min_index);
    }

    void selection_sort(std::vector<int>& arr) {
        for (int i = 0; i < arr.size() - 1; ++i) {
            std::promise<int> prom;
            std::future<int> fut = prom.get_future();

            std::async(std::launch::async, find_min_index, std::cref(arr), i, std::move(prom));

            int min_index = fut.get();

            if (min_index != i)
                std::swap(arr[i], arr[min_index]);
        }
    }
    
    void start() {
        std::vector<int> list = { 64, 25, 12, 22, 11 };
        selection_sort(list);
        for (int num : list) {
            std::cout << num << " ";
        }
    }
}

namespace second_task {

    const int MIN_BLOCK_SIZE = 1000;
    template<typename Iterator, typename Function>
    void parallel_for_each(Iterator begin, Iterator end, Function func) {
        int len = std::distance(begin, end);

        if (len <= MIN_BLOCK_SIZE) {
            std::for_each(begin, end, func);
        }
        else {
            Iterator mid = begin + len / 2;

            auto left_future = std::async(std::launch::async, parallel_for_each<Iterator, Function>, begin, mid, func);

            parallel_for_each(mid, end, func);

            left_future.get();
        }
    }

    void start() {
        std::vector<int> data(10000, 1);

        parallel_for_each(data.begin(), data.end(), [](int& x) {
            x += 5;
            });

        std::cout << data[0] << " " << data[9999] << std::endl;
    }
}


int main() {
    //first_task::start();
    second_task::start();
	return 0;
}