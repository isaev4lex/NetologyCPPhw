#include <iostream>
#include <array>
#include "math_custom.h"

class UIengine {
private:
    int a, b;

    std::string operations[5] = {
        "сложение",
        "вычитание",
        "умножение",
        "деление",
        "возведение в степень"
    };


    void ask_first_num() {
        std::cout << "Введите первое число: ";
        std::cin >> a;
    }

    void ask_second_num() {
        std::cout << "Введите второе число: ";
        std::cin >> b;
    }

    int choose_operation() {
        int user_s_choice;

        std::cout << "Выберите операцию (";
        for (int i = 0; i < 5; ++i) {
            std::cout << i + 1 << " - " << operations[i];
            if (i < 4) std::cout << ", ";
        }
        std::cout << "): ";

        std::cin >> user_s_choice;
        if (user_s_choice < 1 || user_s_choice >= 6) {
            std::cout << "Вы выбрали неверное значение. Попробуйте снова.\n";
            choose_operation();
        } else {
            return user_s_choice;
        }

    }

public:
    bool start_ui() {
        try {
            ask_first_num();
            ask_second_num();
            return true;
        }
        catch (...) {
            std::cerr << "Неизвестная ошибка. Попробуйте использовать другие значения.";
            return false;
        }
    }

    std::array<int, 2> get_a_b() {
        return { a, b };
    }

    std::string get_operation() {
        return operations[choose_operation() - 1]; // Чтобы считать не с 0 а с 1.
    }

};

int main()
{
    setlocale(LC_ALL, "Russian");

    UIengine ui_obj;
    ui_obj.start_ui();

    std::string operation_name = ui_obj.get_operation();
    std::array <int, 2> values = ui_obj.get_a_b(); // Решил просто попробовать сделать именно такой массив. 
    std::cout << mathmain(operation_name, values[0], values[1]) << std::endl;

    main();
}
