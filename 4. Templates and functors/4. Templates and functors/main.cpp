#include <iostream>
#include <vector>


// Simple nums
template <typename T>
T square(const T & num) {
	return num * num;
}

// Vectors
template <typename T>
std::vector<T> square(const std::vector<T> &vctr) {
	std::vector<T> result;
	for (const T &num : vctr) {
		result.push_back(num * num);
	}
    return result;
}

void first_task() {
    int num = 9;
    std::cout << "[IN]: " << num << "\n";
    std::cout << "[OUT]: " << square(num) << "\n";

    std::vector<int> numbers = { 1, 2, 8 };
    std::cout << "[IN]: ";
    for (const int& n : numbers) {
        std::cout << n << ", ";
    }
    std::cout << std::endl;;

    std::vector<int> squaredNumbers = square(numbers);
    std::cout << "[OUT]: ";
    for (const int& n : squaredNumbers) {
        std::cout << n << ", ";
    }

    std::cout << std::endl;
}


template<typename T>
class Table {
private:
    int x;
    int y;
    std::vector<std::vector<T>> arr;

public:
    Table(int x, int y) : x(x), y(y), arr(x, std::vector<T>(y)) {}

    std::vector<T>& operator[](int index) {
        return arr[index];
    }

    const std::vector<T>& operator[](int index) const {
        return arr[index];
    }

    std::pair<int, int> Size() const {
        return { x, y };
    }
};


void second_task() {
    auto test = Table<int>(2, 3);
    test[0][0] = 4;
    std::cout << test[0][0]; // выводит 4
}

class ThirdTaskCounter {
private:
    int sum = 0;
    int count = 0;
public:
    void operator()(int num) {
        if (num % 3 == 0) {
            sum += num;
            count++;
        }
    }

    int get_sum() {
        return sum;
    }

    int get_count() {
        return count;
    }
};

void third_task() {
    int my_arr[6] = { 4, 1, 3, 6, 25, 54 };
    int i = 0;

    ThirdTaskCounter counter;
    while (i != 6) {
        counter(my_arr[i]);
        i++;
    }

    std::cout << "get_sum() = " << counter.get_sum() << std::endl;
    std::cout << "get_count() = " << counter.get_count() << std::endl;
}

int main(int argv, char** argc) {
    
    //first_task();
    //second_task();
    third_task();

	return 0;
}