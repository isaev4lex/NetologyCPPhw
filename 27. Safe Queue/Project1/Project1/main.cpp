#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <sstream>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <future>
#include <atomic>
#include <chrono>


template <typename T>
class safe_queue {
private:
    std::queue<T> queue;
    std::mutex mtx;
    std::condition_variable cv;
public:
    safe_queue() = default;

    void push(T value) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            queue.push(std::move(value));
        }
        cv.notify_one();
    }

    bool pop(T& result) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !queue.empty(); });
        result = std::move(queue.front());
        queue.pop();
        return true;
    }
};

class thread_pool {
private:
    std::vector<std::thread> workers;
    safe_queue<std::function<void()>> tasks;
    std::atomic<bool> stop;

    void work() {
        while (!stop) {
            std::function<void()> task;
            if (tasks.pop(task)) {
                task();
            }
        }
    }

public:
    thread_pool() : stop(false) {
        unsigned int thread_count = std::thread::hardware_concurrency();
        for (unsigned int i = 0; i < thread_count; ++i) {
            workers.emplace_back([this] { work(); });
        }
    }

    ~thread_pool() {
        stop = true;
        for (size_t i = 0; i < workers.size(); ++i) {
            tasks.push([] {});
        }
        for (auto& thread : workers) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    template <typename F, typename... Args>
    auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {
        using return_type = decltype(f(args...));

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        tasks.push([task]() { (*task)(); });
        return task->get_future();
    }
};

std::mutex cout_mutex;

void test_task(const std::string& name) {
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Running: " << name << " on thread " << std::this_thread::get_id() << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Finished: " << name << std::endl;
    }
}

int main() {
    thread_pool pool;

    for (int i = 0; i < 10; ++i) {
        std::stringstream ss;
        ss << "Task A " << i;
        std::string taskA = ss.str();

        ss.str(""); ss.clear();
        ss << "Task B " << i;
        std::string taskB = ss.str();

        pool.submit(test_task, taskA);
        pool.submit(test_task, taskB);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}
