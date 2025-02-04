#include "Quadrangle.h"
#include <iostream>

Quadrangle::Quadrangle(int a, int b, int c, int d, int A, int B, int C, int D)
    : Figure("Четырёхугольник"), a(a), b(b), c(c), d(d), A(A), B(B), C(C), D(D) {}

void Quadrangle::print_info() const {
    std::cout << name << ":\n"
        << "Стороны: a=" << a << " b=" << b << " c=" << c << " d=" << d << "\n"
        << "Углы: A=" << A << " B=" << B << " C=" << C << " D=" << D << "\n";
}