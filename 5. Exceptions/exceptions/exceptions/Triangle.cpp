#include "Triangle.h"

Triangle::Triangle(double side1, double side2, double side3, int angle1, int angle2, int angle3) : a(side1), b(side2), c(side3), A(angle1), B(angle2), C(angle3) {

    if (A + B + C != 180) {
        throw FigureCreationError("������ �������� ������. �������: ����� ����� �� ����� 180.");
    }

    std::cout << "����������� ������: ������� (" << a << ", " << b << ", " << c
        << "); ���� (" << A << ", " << B << ", " << C << ")\n";
}

void Triangle::display() const {
    std::cout << "�����������: ������� (" << a << ", " << b << ", " << c << "), ���� (" << A << ", " << B << ", " << C << ")\n";
}
