#include <iostream>
#include <string>

int main() {
    std::string name;

    std::cout << "Введите имя: ";
    std::getline(std::cin, name);

    std::cout << "Здравствуйте, " << name << "!" << std::endl;

    return 0;
}
