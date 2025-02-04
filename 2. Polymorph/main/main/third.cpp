#include <iostream>
#include <string>

namespace third {

    class Figure {
    protected:
        std::string name;

    public:
        Figure(const std::string& name) : name(name) {}

        virtual void print_info() const {
            std::cout << name << ":\n"
                << "����������: " << (check() ? "��" : "���") << "\n"
                << "���������� ������: 0\n";
        }

        virtual bool check() const {
            return true;
        }

        virtual ~Figure() = default;
    };

    class Triangle : public Figure {
    protected:
        int a, b, c;
        int A, B, C;

    public:
        Triangle(int a, int b, int c, int A, int B, int C) : Figure("�����������"), a(a), b(b), c(c), A(A), B(B), C(C) {}

        bool check() const override {
            return (A + B + C == 180);
        }

        void print_info() const override {
            std::cout << name << ":\n"
                << "����������: " << (check() ? "��" : "���") << "\n"
                << "���������� ������: 3\n"
                << "�������: a=" << a << " b=" << b << " c=" << c << "\n"
                << "����: A=" << A << " B=" << B << " C=" << C << "\n";
        }
    };

    class RightTriangle : public Triangle {
    public:
        RightTriangle(int a, int b, int c, int A, int B)
            : Triangle(a, b, c, A, B, 90) {
            name = "������������� �����������";
        }
    };

    class IsoscelesTriangle : public Triangle {
    public:
        IsoscelesTriangle(int a, int b, int A, int B)
            : Triangle(a, b, a, A, B, A) {
            name = "�������������� �����������";
        }
    };

    class EquilateralTriangle : public Triangle {
    public:
        EquilateralTriangle(int side)
            : Triangle(side, side, side, 60, 60, 60) {
            name = "�������������� �����������";
        }
    };

    class Quadrangle : public Figure {
    protected:
        int a, b, c, d;
        int A, B, C, D;

    public:
        Quadrangle(int a, int b, int c, int d, int A, int B, int C, int D) : Figure("��������������"), a(a), b(b), c(c), d(d), A(A), B(B), C(C), D(D) {}

        bool check() const override {
            return (A + B + C + D == 360);
        }

        void print_info() const override {
            std::cout << name << ":\n"
                << "����������: " << (check() ? "��" : "���") << "\n"
                << "���������� ������: 4\n"
                << "�������: a=" << a << " b=" << b << " c=" << c << " d=" << d << "\n"
                << "����: A=" << A << " B=" << B << " C=" << C << " D=" << D << "\n";
        }
    };

    class Rectangle : public Quadrangle {
    public:
        Rectangle(int a, int b) : Quadrangle(a, b, a, b, 90, 90, 90, 90) {
            name = "�������������";
        }
    };

    class Square : public Rectangle {
    public:
        Square(int side) : Rectangle(side, side) {
            name = "�������";
        }
    };

    class Parallelogram : public Quadrangle {
    public:
        Parallelogram(int a, int b, int A, int B) : Quadrangle(a, b, a, b, A, B, A, B) {
            name = "��������������";
        }
    };

    class Romb : public Parallelogram {
    public:
        Romb(int side, int A, int B)
            : Parallelogram(side, side, A, B) {
            name = "����";
        }
    };

    void print_info(const Figure* figure) {
        figure->print_info();
    }

    int third_task_main() {
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

        Figure* figures[9] = {
            &triangle,
            &right_triangle,
            &isosceles_triangle,
            &equilateral_triangle,
            &quadrangle,
            &rectangle,
            &square,
            &parallelogram,
            &romb
        };

        for (int i = 0; i < 9; ++i) {
            std::cout << std::endl;
            print_info(figures[i]);
        }

        return 0;
    }
};
