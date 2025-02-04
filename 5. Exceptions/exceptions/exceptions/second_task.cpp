#include "Triangle.h"
#include <iostream>

namespace second_task {
    void start_second_task() {
        try {
            Triangle t1(3, 4, 5, 30, 60, 90);
            t1.display();
        }
        catch (const FigureCreationError& e) {
            std::cerr << e.what() << '\n';
        }

        try {
            Triangle t2(3, 4, 5, 50, 60, 70);
            t2.display();
        }
        catch (const FigureCreationError& e) {
            std::cerr << e.what() << '\n';
        }
    }
}

