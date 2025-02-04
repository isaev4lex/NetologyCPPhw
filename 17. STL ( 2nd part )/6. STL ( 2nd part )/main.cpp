#include "allinclude.h"

namespace first_task {
	void start() {
		std::vector<int> myvector = { 1, 1, 2, 5, 6, 1, 2, 4 };

		std::cout << "[IN]";
		for (auto i : myvector) {
			std::cout << " " << i;
		}
		std::cout << std::endl;

		std::sort(myvector.begin(), myvector.end());
		auto vecuniq = std::unique(myvector.begin(), myvector.end());
		myvector.erase(vecuniq, myvector.end());


		std::cout << "[OUT]";
		for (auto i : myvector) {
			std::cout << " " << i;
		}
		std::cout << std::endl;
	}
}

namespace second_task {
	template <typename T>
	void print_container(const T &t) {
		auto it = t.begin();
		while (it != t.end()) {
			std::cout << *it << " ";
			it++;
		}
		std::cout << std::endl;
	}

	void start() {
		std::set<std::string> test_set = { "one", "two", "three", "four" };
		print_container(test_set); // four one three two. помните почему такой порядок? :)

		std::list<std::string> test_list = { "one", "two", "three", "four" };
		print_container(test_list); // one, two, three, four

		std::vector<std::string> test_vector = { "one", "two", "three", "four" };
		print_container(test_vector); // one, two, three, four
	}
}

int main(int argc, char** argv) {

	//first_task::start();
	second_task::start();

	return 0;
}