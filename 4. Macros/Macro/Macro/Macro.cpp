#include <iostream>
#include "second_task.h"

#define MODE 1

#ifndef MODE
#error "You must specify mode"
#endif

#define add(one, two) ((one + two))

void first_task() {
	if (MODE == 0) {
		std::cout << "Работаю в режиме тренировки" << std::endl;
	}
	else if (MODE == 1) {
		std::cout << "Работаю в боевом режиме" << std::endl;
		std::cout << "Введите число 1: ";
		int first_num;
		std::cin >> first_num;
		std::cout << "Введите число 2: ";
		int second_num;
		std::cin >> second_num;
		std::cout << "Результат сложения: " << add(first_num, second_num) << std::endl;
	}
}

int main() {
	setlocale(LC_ALL, "Russian");

	//first_task();
	second_task();
	
	return 0;
}