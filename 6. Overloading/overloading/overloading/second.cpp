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

    Fraction operator+(const Fraction& obj) const {
        return Fraction(numerator_ * obj.denominator_ + obj.numerator_ * denominator_, denominator_ * obj.denominator_);
    }
    Fraction operator-(const Fraction& obj) const {
        return Fraction(numerator_ * obj.denominator_ - obj.numerator_ * denominator_, denominator_ * obj.denominator_);
    }
    Fraction operator*(const Fraction& obj) const {
        return Fraction(numerator_ * obj.numerator_, denominator_ * obj.denominator_);
    }
    Fraction operator/(const Fraction& obj) const {
        if (obj.numerator_ == 0) {
            throw std::invalid_argument("Division by zero");
        }
        return Fraction(numerator_ * obj.denominator_, denominator_ * obj.numerator_);
    }
    Fraction operator++(int) {
        Fraction temp = *this;
        numerator_ += denominator_;
        return temp;
    }

    Fraction operator--(int) {
        Fraction temp = *this;
        numerator_ -= denominator_;
        return temp;
    }

    Fraction& operator++() {
        numerator_ += denominator_;
        return *this;
    }

    Fraction& operator--() {
        numerator_ -= denominator_;
        return *this;
    }
    friend std::ostream& operator<<(std::ostream& os, const Fraction& obj) {
        os << obj.numerator_ << "/" << obj.denominator_;
        return os;
    }
};

int* get_nums(int ask_time) {
    int* user_nums = new int[2];

    std::cout << "¬ведите числитель дроби " << ask_time << ": ";
    std::cin >> user_nums[0];

    std::cout << "¬ведите знаменатель дроби " << ask_time << ": ";
    std::cin >> user_nums[1];
    return user_nums;

}

int second_task()
{
    int* first_obj_data = get_nums(1);
    int* second_obj_data = get_nums(2);

    Fraction f1(first_obj_data[0], first_obj_data[1]);
    Fraction f2(second_obj_data[0], second_obj_data[1]);

    std::cout << f1 << " + " << f2 << " = " << f1 + f2 << "\n";
    std::cout << f1 << " - " << f2 << " = " << f1 - f2 << "\n";
    std::cout << f1 << " * " << f2 << " = " << f1 * f2 << "\n";
    std::cout << f1 << " / " << f2 << " = " << f1 / f2 << "\n";

     Fraction f1_copy = f1;
    std::cout << "++" << f1 << " * " << f2 << " = " << (++f1 * f2) << "\n";
    std::cout << "«начение дроби 1 = " << f1 << "\n";

    std::cout << f1_copy << "-- * " << f2 << " = " << (f1_copy-- * f2) << "\n";
    std::cout << "«начение дроби 1 = " << f1_copy << "\n";

    delete[] first_obj_data;
    delete[] second_obj_data;

    return 0;
}