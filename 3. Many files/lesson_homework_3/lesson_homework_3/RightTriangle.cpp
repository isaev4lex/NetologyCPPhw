#include "RightTriangle.h"

RightTriangle::RightTriangle(int a, int b, int c, int A, int B)
    : Triangle(a, b, c, A, B, 90) {
    name = "Прямоугольный треугольник";
}