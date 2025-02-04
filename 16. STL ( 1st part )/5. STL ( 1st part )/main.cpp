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

namespace second_task {
    std::vector<int> filter_unique(const std::vector<int>& input) {
        std::set<int> unique_nums(input.begin(), input.end());
        return std::vector<int>(unique_nums.rbegin(), unique_nums.rend());
    }

    void second_task() {
        std::string user_input;
        std::cout << "[IN]:" << std::endl;

        try {
            std::getline(std::cin, user_input);
            int count = std::stoi(user_input);

            if (count <= 0) {
                throw std::invalid_argument("Must be greater than zero!");
            }

            std::vector<int> user_nums;

            for (int i = 0; i < count; ++i) {
                std::getline(std::cin, user_input);
                try {
                    int new_num = std::stoi(user_input);
                    user_nums.push_back(new_num);
                }
                catch (const std::exception& e) {
                    std::cerr << "Invalid input: " << e.what() << std::endl;
                }
            }

            std::vector<int> unique_nums = filter_unique(user_nums);

            std::cout << "[OUT]:" << std::endl;
            for (int num : unique_nums) {
                std::cout << num << std::endl;
            }

        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
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
    second_task::second_task();

    /*thirdtask::AnalogVector<std::string> myanalog_vector;
    myanalog_vector.push_back("Hello");
    myanalog_vector.push_back("World");
    myanalog_vector.push_back("!");

    std::cout << "AnalogVector 2nd index -  " << myanalog_vector.at(2) << std::endl;
    std::cout << "AnalogVector size: " << myanalog_vector.size() << std::endl;
    std::cout << "AnalogVector capacity: " << myanalog_vector.capacity() << std::endl;*/

	return 0;
}