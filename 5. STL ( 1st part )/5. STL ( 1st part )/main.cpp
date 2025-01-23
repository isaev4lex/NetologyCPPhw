#include "allinclude.h"

void first_task() {
	std::map<char, int> result;

	std::string user_input;
	std::cout << "[IN]: ";
	std::getline(std::cin, user_input);

	for (char c : user_input) {
		result[c]++;
	} // Я не делаю тут проверку на большую или маленькую букву, ибо подразумевается подсчёт буквы зависимо от её регистра

	std::vector<std::pair<char, int>> sorted_result(result.begin(), result.end());
	std::sort(sorted_result.begin(), sorted_result.end(), [](const std::pair<char, int>& a, const std::pair<char, int>& b) {
		return a.second > b.second;
		}
	);

	std::cout << "[OUT]:" << std::endl;
	for (const auto& pair : sorted_result) {
		std::cout << pair.first << ": " << pair.second << std::endl;
	}
}

void second_task() {
    std::string user_input;
    std::cout << "[IN]:" << std::endl;
    std::getline(std::cin, user_input);

    try {
        int count = std::stoi(user_input);

        if (count <= 0) {
            throw std::invalid_argument("Must be greater than zero!");
        }

        std::set<int> user_nums;
        int i = 0;

        while (i < count) {
            std::getline(std::cin, user_input);

            try {
                int new_num = std::stoi(user_input);
                user_nums.insert(new_num);
            }
            catch (std::exception& e) {
                std::cerr << e.what() << std::endl;
            }

            i++;
        }

        std::cout << "[OUT]:" << std::endl;
        for (auto j = user_nums.rbegin(); j != user_nums.rend(); j++) {
            std::cout << *j << std::endl;
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

namespace thirdtask {
    template <typename T>
    class AnalogVector {
    private:
        T* data;
        int _size;
        int _capacity;

        void resize(int newcapacity) {
            T* new_data = new T[newcapacity];

            for (int i = 0; i < _size; ++i) {
                new_data[i] = data[i];
            }

            delete[] data;
            data = new_data;
            _capacity = newcapacity;
        }

    public:
        AnalogVector() : data(nullptr), _size(0), _capacity(0) {}

        T& at(int index) {
            if (index < 0 || index >= _size) {
                throw std::out_of_range("Out of range");
            }
            return data[index];
        }

        void push_back(T value) {
            if (_size == _capacity) {
                int newcapacity = (_capacity == 0) ? 1 : _capacity * 2;
                resize(newcapacity);
            }
            data[_size] = value;
            ++_size;
        }

        int size() const {
            return _size;
        }

        int capacity() const {
            return _capacity;
        }

        ~AnalogVector() {
            delete[] data;
        }
    };
}



int main(int argv, char** argc) {

	//first_task();
    //second_task();

    thirdtask::AnalogVector<std::string> myanalog_vector;
    myanalog_vector.push_back("Hello");
    myanalog_vector.push_back("World");
    myanalog_vector.push_back("!");

    std::cout << "AnalogVector 2nd index -  " << myanalog_vector.at(2) << std::endl;
    std::cout << "AnalogVector size: " << myanalog_vector.size() << std::endl;
    std::cout << "AnalogVector capacity: " << myanalog_vector.capacity() << std::endl;

	return 0;
}