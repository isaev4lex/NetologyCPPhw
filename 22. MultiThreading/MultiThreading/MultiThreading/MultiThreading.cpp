#include "allinclude.h"
#include "FirstTask.h"
#include "SecondTask.h"

//void create_random_vector(std::vector<int>& V) {
//	std::mt19937 gen;
//	std::uniform_int_distribution<int> dis(1, 1'000'000);
//	auto rand_number([=]() mutable {return dis(gen); });
//	std::generate(V.begin(), V.end(), rand_number);
//}
//
//void Sort(std::vector<int>& v) {
//
//	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
//
//	std::sort(v.begin(), v.end());
//
//	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
//
//	std::chrono::duration<double, std::milli> time = end - start;
//
//	std::cout << "std::sort time = " << time.count() << " ms" << std::endl;
//}
//
//void ParrallelSort(std::vector<int>& v) {
//
//	std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
//
//	std::sort(std::execution::par, v.begin(), v.end());
//
//	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
//
//	std::chrono::duration<double, std::milli> time = end - start;
//
//	std::cout << "std::sort time = " << time.count() << " ms" << std::endl;
//}


//void func1() {
//	for (int i = 0; i < 10; ++i) {
//		std::cout << i << " ";
//	}
//	std::cout << std::endl;
//}
//
//void func2() {
//	std::string str[10] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" };
//	for (int i = 0; i < 10; ++i) {
//		std::this_thread::sleep_for(std::chrono::milliseconds(100));
//		std::cout << str[i] << " ";
//	}
//	std::cout << std::endl;
//}
//
//
//class TestClass {
//private:
//	std::thread t4;
//public:
//	TestClass() {
//		t4  = std::thread(&TestClass::show_info, this);
//	}
//
//	void show_info() {
//		std::cout << "TestClass show_info" << std::endl;
//	}
//
//	~TestClass() {
//		if (t4.joinable()) {
//			t4.join();
//		}
//		std::cout << "TestClass destructor" << std::endl;
//	}
//
//};
//
  
//void show_sum(int a, int b) {
//	std::cout << "Sum = " << a + b << std::endl;
//}


void first_task() {
	try {
		QueueImitation queueImitation(20);
		queueImitation.start();
	}
	catch (const std::exception& e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	catch (...) {
		std::cerr << "Unknown exception occurred." << std::endl;
	}
}

void second_task() {
	VectorAdder adder;
	adder.run();

	/*Hardware threads available : 32

		Size        Threads   Time(ms)
		--------------------------------------
		1000        2         0.6458
		1000        4         0.2861
		1000        8         0.5461
		1000        16        0.7898
		10000       2         0.1449
		10000       4         0.2135
		10000       8         0.4357
		10000       16        0.7868
		100000      2         0.4401
		100000      4         0.4873
		100000      8         0.5554
		100000      16        0.6363
		1000000     2         2.5542
		1000000     4         1.6484
		1000000     8         1.8824
		1000000     16        2.2183*/
}

int main() {

	/*std::vector<int> V(1'000'000);
	create_random_vector(V);
	Sort(V);

	std::vector<int> V1(1'000'000);
	create_random_vector(V1);
	ParrallelSort(V1);*/

	//func1();
	//func2();

	//std::cout << std::thread::hardware_concurrency() << std::endl; // 32

	/*std::thread t1(func1);
	std::thread t2(func2);

	if (t1.joinable()) {
		t1.join();
	}

	if (t2.joinable()) {
		t2.join();
	}

	std::thread t3(
		[]() {
			for (int i = 0; i < 10; ++i) {
				std::cout << i << " ";
			}
			std::cout << std::endl;
		}
	);

	t3.join();

	TestClass();*/
	
	/*std::thread t5;
	t5 = std::thread(show_sum, 10, 20);
	t5.join();*/

	//first_task();
	second_task();

	return 0;
}
