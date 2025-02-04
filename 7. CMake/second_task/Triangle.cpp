#include "Triangle.h"
#include <iostream>

Triangle::Triangle(int a, int b, int c, int A, int B, int C)
    : Figure("Треугольник"), a(a), b(b), c(c), A(A), B(B), C(C) {}

void Triangle::print_info() const {
    std::cout << name << ":\n"
        << "Стороны: a=" << a << " b=" << b << " c=" << c << "\n"
        << "Углы: A=" << A << " B=" << B << " C=" << C << "\n";
}