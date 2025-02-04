#include <iostream>
#include <cmath>
#include <string>

class MathEngine {
private:
	int a, b;
public:
	MathEngine(int a, int b) {
		this->a = a;
		this->b = b;
	}

	int plus() {
		return a + b;
	}

	int minus() {
		return a - b;
	}

	int multiply() {
		return a * b;
	}

	int divide() {
		return a / b;
	}

	int exponentiation() {
		return pow(a, b);
	}
};

std::string mathmain(std::string operation, int a, int b) {

	MathEngine math_engine(a, b);

	if (operation == "��������") {
		return std::to_string(a) + " + " + std::to_string(b) + " = " + std::to_string(math_engine.plus());
	}
	else if (operation == "���������") {
		return std::to_string(a) + " - " + std::to_string(b) + " = " + std::to_string(math_engine.minus());
	}
	else if (operation == "���������") {
		return std::to_string(a) + " * " + std::to_string(b) + " = " + std::to_string(math_engine.multiply());
	}
	else if (operation == "�������") {
		if (b == 0) {
			return "�� 0 ������ ������!";
		} else {
			return std::to_string(a) + " / " + std::to_string(b) + " = " + std::to_string(math_engine.divide());
		}
	}
	else if (operation == "���������� � �������") {
		return std::to_string(a) + " � ������� " + std::to_string(b) + " = " + std::to_string(math_engine.exponentiation());
	}
	else {
		std::cerr << "� �������������� ������� �������� ����������� ��������" << std::endl;
		return "";
	}

	return 0;
}
