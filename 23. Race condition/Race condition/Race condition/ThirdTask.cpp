#include "ThirdTask.h"

Data::Data(int v, const std::string& t) : value(v), text(t) {}

void Data::print(const std::string& prefix) const {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << prefix << "Value: " << value << ", Text: " << text << '\n';
}

void swap_with_lock(Data& a, Data& b) {
    if (&a == &b) return;

    std::lock(a.mtx, b.mtx);

    std::lock_guard<std::mutex> lock1(a.mtx, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(b.mtx, std::adopt_lock);

    std::swap(a.value, b.value);
    std::swap(a.text, b.text);
}


void swap_with_scoped_lock(Data& a, Data& b) {
    if (&a == &b) return;

    std::scoped_lock lock(a.mtx, b.mtx);
    std::swap(a.value, b.value);
    std::swap(a.text, b.text);
}

void swap_with_unique_lock(Data& a, Data& b) {
    if (&a == &b) return;

    std::unique_lock<std::mutex> lock1(a.mtx, std::defer_lock);
    std::unique_lock<std::mutex> lock2(b.mtx, std::defer_lock);

    std::lock(lock1, lock2);

    std::swap(a.value, b.value);
    std::swap(a.text, b.text);
}