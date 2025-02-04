#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "FigureCreationError.h"
#include <iostream>

class Triangle {
private:
    double a, b, c;
    int A, B, C;

public:
    Triangle(double side1, double side2, double side3, int angle1, int angle2, int angle3);
    void display() const;
};

#endif