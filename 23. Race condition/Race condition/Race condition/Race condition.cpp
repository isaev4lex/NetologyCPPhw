#include "allinclude.h"
#include "FirstTask.h"
#include "SecondTask.h"
#include "ThirdTask.h"

//void func1() {
//    //mtx.lock();
//    //std::lock_guard<std::mutex> lock(mtx);
//    std::unique_lock<std::mutex> unique_lock(mtx);
//    for (int i = 0; i != 10; i++) {
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//        std::cout << i << " ";
//    }
//    std::cout << std::endl;
//    unique_lock.unlock();
//	//mtx.unlock();
//}
//
//void func2() {
//    //mtx.lock();
//    //std::lock_guard<std::mutex> lock(mtx);
//    std::unique_lock<std::mutex> unique_lock(mtx);
//    char char_arr[10] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };
//    for (int i = 0; i != 10; ++i) {
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
//        std::cout << char_arr[i] << " ";
//    }
//    std::cout << std::endl;
//    unique_lock.unlock();
//    //mtx.unlock();
//}
//
// 


void task_1() {
    QueueImitation queue(12);
    queue.start();
}

void task_2() {
    CalculationImitation second_task(30, 5, 100);
    second_task.run();
}

void task_3() {
    {
        Data d1(1, "One");
        Data d2(2, "Two");

        std::cout << "=== swap_with_lock ===\n";
        std::cout << "Before swap:\n";
        d1.print("d1: ");
        d2.print("d2: ");

        swap_with_lock(d1, d2);

        std::cout << "After swap:\n";
        d1.print("d1: ");
        d2.print("d2: ");
        std::cout << "\n";
    }

    {
        Data d1(3, "Three");
        Data d2(4, "Four");

        std::cout << "=== swap_with_scoped_lock ===\n";
        std::cout << "Before swap:\n";
        d1.print("d1: ");
        d2.print("d2: ");

        swap_with_scoped_lock(d1, d2);

        std::cout << "After swap:\n";
        d1.print("d1: ");
        d2.print("d2: ");
        std::cout << "\n";
    }

    {
        Data d1(5, "Five");
        Data d2(6, "Six");

        std::cout << "=== swap_with_unique_lock ===\n";
        std::cout << "Before swap:\n";
        d1.print("d1: ");
        d2.print("d2: ");

        swap_with_unique_lock(d1, d2);

        std::cout << "After swap:\n";
        d1.print("d1: ");
        d2.print("d2: ");
        std::cout << "\n";
    }
}

int main() {
    /*std::thread t1(func1);
	std::thread t2(func2);

	t1.join();
	t2.join();

*/
    task_1();
    task_2();
    task_3();

    return 0;
}