#include <iostream>
#include <string>


class Figure {
private:
    int sides_count;
    std::string name;
protected:
    Figure(int sides_count, std::string name) {
        this->sides_count = sides_count;
        this->name = name;
    }
public:
    int get_sides_count() const {
        return sides_count;
    }

    std::string get_name() const {
        return name;
    }
};

class Triangle : public Figure {
public:
    Triangle() : Figure(3, "Треугольник") {}
};

class Quadrangle : public Figure {
public:
    Quadrangle() : Figure(4, "Четырёхугольник") {}
};

class UnknownFigure : public Figure {
public:
    UnknownFigure() : Figure(0, "Фигура") {}
};

int first_task_main() {
    setlocale(LC_ALL, "Russian");

    UnknownFigure unknown;
    Triangle triangle;
    Quadrangle quadrangle;

    std::cout << "Количество сторон:" << std::endl;
    std::cout << unknown.get_name() << ": " << unknown.get_sides_count() << std::endl;
    std::cout << triangle.get_name() << ": " << triangle.get_sides_count() << std::endl;
    std::cout << quadrangle.get_name() << ": " << quadrangle.get_sides_count() << std::endl;

    return 0;
}
