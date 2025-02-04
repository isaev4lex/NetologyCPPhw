#include <iostream>
#include <stdexcept>

class bad_length : public std::exception {
public:
	virtual const char* what() const noexcept override{
		return "Вы ввели слово запретной длины! До свидания";
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
			return 7522875; /* Cделал именно такое значение, ибо мне не понятно, зачем эта функция должна возвращать
			именно длину строки, если она может быть равна тому же, что и при случае пойманной ошибке (т.к. я 
			обязан хоть что-то вернуть. А это что-то - это int, а этот инт может быть такой-же как и длина строки
			тогда я запутаюсь, когда я буду работать с этой функцией. Но я всё же сделал как требуется. */
		}
	}

	void start_first_task() noexcept {
		std::cout << "Введите запретную длину: ";
		int forbidden_length;
		std::cin >> forbidden_length;
		
		while (true) {
			std::cout << "Введите слово: ";
			std::string mystr;
			std::cin >> mystr;

			int func_out = function(mystr, forbidden_length);

			if (func_out == 7522875) {
				break;
			} else {
				std::cout << "Длина слова " << mystr << " равна " << func_out << std::endl;
			}
		}
	}
}

