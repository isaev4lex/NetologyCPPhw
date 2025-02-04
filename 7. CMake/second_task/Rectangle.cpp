#include "Rectangle.h"

Rectangle::Rectangle(int a, int b)
    : Quadrangle(a, b, a, b, 90, 90, 90, 90) {
    name = "Прямоугольник";
}