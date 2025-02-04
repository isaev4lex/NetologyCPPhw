#include <iostream>
#include "Triangle.h"
#include "RightTriangle.h"
#include "IsoscelesTriangle.h"
#include "EquilateralTriangle.h"
#include "Quadrangle.h"
#include "Rectangle.h"
#include "Square.h"
#include "Parallelogram.h"
#include "Romb.h"

void print_info(const Figure* figure) {
    figure->print_info();
}

int main() {
    setlocale(LC_ALL, "Russian");

    Triangle triangle(10, 20, 30, 50, 60, 70);
    RightTriangle right_triangle(10, 20, 30, 50, 60);
    IsoscelesTriangle isosceles_triangle(10, 20, 50, 60);
    EquilateralTriangle equilateral_triangle(30);
    Quadrangle quadrangle(10, 20, 30, 40, 50, 60, 70, 80);
    Rectangle rectangle(10, 20);
    Square square(20);
    Parallelogram parallelogram(20, 30, 30, 40);
    Romb romb(30, 30, 40);

    Figure* figures[9];
    figures[0] = &triangle;
    figures[1] = &right_triangle;
    figures[2] = &isosceles_triangle;
    figures[3] = &equilateral_triangle;
    figures[4] = &quadrangle;
    figures[5] = &rectangle;
    figures[6] = &square;
    figures[7] = &parallelogram;
    figures[8] = &romb;

    for (int i = 0; i < 9; ++i) {
        std::cout << std::endl;
        print_info(figures[i]);
    }

    return 0;
}