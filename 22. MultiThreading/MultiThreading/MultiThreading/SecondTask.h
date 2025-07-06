#pragma once 
#include "allinclude.h"


class VectorAdder {
public:
    void run();
private:
    double measure_sum(int size, int thread_count);
};
