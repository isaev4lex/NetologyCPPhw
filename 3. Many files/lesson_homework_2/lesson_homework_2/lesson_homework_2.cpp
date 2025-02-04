#include <iostream>
#include <array>
#include <string>
#include "counter.h"

class UIengine {
private:
    int start_val;

    void set_start_val() {
        //std::cout << "Вы хотите указать начальное значение счётчика? Введите да или нет: ";
        std::cout << "Are you want to set started value of the counter? Enter yes or no: ";
        std::string users_answer;
        std::cin >> users_answer;

        for (char& c : users_answer) {
            c = std::tolower(c);
        }

        //if (users_answer == "да") {
        if (users_answer == "yes") {
            std::cout << "Enter started value of the counter: ";
            int counter_val;
            std::cin >> counter_val;
            start_val = counter_val;
        //} else if(users_answer == "нет") {
        } else if(users_answer == "no") {
            start_val = 0;
        } else {
            //std::cout << "Вы ввели неверное значение. Попробуйте снова..." << std::endl;
            std::cout << "You've entered invalid value. Try again..." << std::endl;
            set_start_val();
        }
    }
public:
    int get_start_val() {
        set_start_val();
        return start_val;
    }
    
};

int main()
{
    //setlocale(LC_ALL, "Russian");

    UIengine uiengine;
    Counter counter;
    std::string command;

    int started_value = uiengine.get_start_val();
    while (counter.show_counter_value() != started_value) {
        counter.increase_counter();
    }

    while (true) {
        //std::cout << "Введите команду ('+', '-', '=' или 'x'): ";
        std::cout << "Enter command ('+', '-', '=' or 'x'): ";
        std::cin >> command;

        if (command == "+") {
            counter.increase_counter();
        } else if (command == "-") {
            counter.decrease_counter();
        } else if (command == "=") {
            std::cout << counter.show_counter_value() << std::endl;
        } else {
            //std::cout << "До свидания!" << std::endl;
            std::cout << "Bye!" << std::endl;
            break;
        }
    }
}
