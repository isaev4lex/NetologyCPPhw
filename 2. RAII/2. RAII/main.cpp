#include "allinclude.h"
#include "FirstTask.h"


void first_task() {
	try {
		std::cout << "Enter array size >> ";
		int count;
		std::cin >> count;

		if (count <= 0) {
			std::cerr << "Invalid size! Exiting." << std::endl;
		} else {
			SmartArray smart_array(count);

			for (int counter = 0; counter < count; counter++) {
				int tmp_var;
				std::cout << "Enter element " << counter << " of array >> ";
				std::cin >> tmp_var;
				smart_array.set_in_array(counter, tmp_var);
			}

			while (true) {
				std::string user_answer;
				std::cout << "Which element of array you want to see? (enter 'exit' for exit) >> ";
				std::cin >> user_answer;

				if (user_answer == "exit" || user_answer == "Exit") {
					break;
				} else {
					try {
						int int_answer = std::stoi(user_answer);

						if (int_answer >= count) {
							std::cout << "You've entered invalid value. Try again" << std::endl;
							continue;
						}

						std::cout << user_answer << " element of array is: " << smart_array.get_element(int_answer) << std::endl;
					}
					catch (std::exception) {
						std::cout << "You've entered invalid value. Try again" << std::endl;
						continue;
					}
				}
			}
		}
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}


void second_task() {
	try {
		SmartArray arr(5);
		arr.set_in_array(0, 1);
		arr.set_in_array(1, 4);
		arr.set_in_array(2, 155);

		SmartArray new_array(2);
		new_array.set_in_array(0, 44);
		new_array.set_in_array(1, 34);

		arr = new_array;

		std::cout << "First element of arr is: " << arr.get_element(0) << std::endl;
		std::cout << "First element of new_array is: " << new_array.get_element(0) << std::endl;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}


int main(int argv, char** argc) {

	//first_task();
	second_task();

	return 0;
}