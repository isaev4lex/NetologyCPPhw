#include "EquilateralTriangle.h"

EquilateralTriangle::EquilateralTriangle(int side)
    : Triangle(side, side, side, 60, 60, 60) {
    name = "Равносторонний треугольник";
}