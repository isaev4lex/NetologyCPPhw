#pragma once
#include "allinclude.h"

class Data {
private:
    int value;
    std::string text;
    mutable std::mutex mtx;

public:
    Data(int v = 0, const std::string& t = "");

    friend void swap_with_lock(Data& a, Data& b);
    friend void swap_with_scoped_lock(Data& a, Data& b);
    friend void swap_with_unique_lock(Data& a, Data& b);

    void print(const std::string& prefix = "") const;
};

