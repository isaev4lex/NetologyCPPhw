#include <iostream>

class Fraction
{
private:
    int numerator_;
    int denominator_;

public:
    Fraction(int numerator, int denominator) : numerator_(numerator), denominator_(denominator) {
        if (denominator_ == 0) {
            throw std::invalid_argument("Denominator cannot be zero");
        }
    }

    bool operator==(const Fraction& obj) const {
        return numerator_ * obj.denominator_ == denominator_ * obj.numerator_;
    }

    bool operator!=(const Fraction& obj) const {
        return !(*this == obj);
    }

    bool operator<(const Fraction& obj) const {
        return numerator_ * obj.denominator_ < denominator_ * obj.numerator_;
    }

    bool operator>(const Fraction& obj) const {
        return obj < *this;
    }

    bool operator<=(const Fraction& obj) const {
        return !(*this > obj);
    }

    bool operator>=(const Fraction& obj) const {
        return !(*this < obj);
    }
};

int first_task()
{
    Fraction f1(4, 3);
    Fraction f2(6, 11);

    std::cout << "f1" << ((f1 == f2) ? " == " : " not == ") << "f2" << '\n';
    std::cout << "f1" << ((f1 != f2) ? " != " : " not != ") << "f2" << '\n';
    std::cout << "f1" << ((f1 < f2) ? " < " : " not < ") << "f2" << '\n';
    std::cout << "f1" << ((f1 > f2) ? " > " : " not > ") << "f2" << '\n';
    std::cout << "f1" << ((f1 <= f2) ? " <= " : " not <= ") << "f2" << '\n';
    std::cout << "f1" << ((f1 >= f2) ? " >= " : " not >= ") << "f2" << '\n';
    return 0;
}