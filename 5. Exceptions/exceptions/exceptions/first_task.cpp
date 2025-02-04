#include <iostream>
#include <stdexcept>

class bad_length : public std::exception {
public:
	virtual const char* what() const noexcept override{
		return "�� ����� ����� ��������� �����! �� ��������";
	}
};


namespace first_task {
	int function(std::string str, int forbidden_length) {
		int str_length = str.length();
		try {
			if (str_length == forbidden_length) {
				throw bad_length();
			} else {
				return str_length;
			}
		} catch (const bad_length& e) {
			std::cout << e.what() << std::endl;
			return 7522875; /* C����� ������ ����� ��������, ��� ��� �� �������, ����� ��� ������� ������ ����������
			������ ����� ������, ���� ��� ����� ���� ����� ���� ��, ��� � ��� ������ ��������� ������ (�.�. � 
			������ ���� ���-�� �������. � ��� ���-�� - ��� int, � ���� ��� ����� ���� �����-�� ��� � ����� ������
			����� � ���������, ����� � ���� �������� � ���� ��������. �� � �� �� ������ ��� ���������. */
		}
	}

	void start_first_task() noexcept {
		std::cout << "������� ��������� �����: ";
		int forbidden_length;
		std::cin >> forbidden_length;
		
		while (true) {
			std::cout << "������� �����: ";
			std::string mystr;
			std::cin >> mystr;

			int func_out = function(mystr, forbidden_length);

			if (func_out == 7522875) {
				break;
			} else {
				std::cout << "����� ����� " << mystr << " ����� " << func_out << std::endl;
			}
		}
	}
}

