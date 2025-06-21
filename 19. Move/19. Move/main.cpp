#include <iostream>
#include <vector>

namespace first_task {
	template <typename T>
	void move_vector(std::vector<T>& a, std::vector<T>& b) {
		a = std::move(b);
	}

	void start() {
		std::vector <std::string> one = { "test_string1", "test_string2" };
		std::vector <std::string> two;

		move_vector(two, one);

		for (std::string i : two) {
			std::cout << i << std::endl;
		}

		for (std::string i : one) {
			std::cout << i << std::endl;
		}
	}
}

namespace second_task {
	class BigNumber {
	private:
		std::vector<int> big_num;

	public:
		BigNumber(std::string new_num) {
			for (char i : new_num) {
				big_num.push_back(i - '0');
			}
		}

		BigNumber(const std::vector<int>& digits) : big_num(digits) {}

		BigNumber(const BigNumber& another_big) : big_num(another_big.big_num) {}

		BigNumber(BigNumber&& another_big) noexcept : big_num(std::move(another_big.big_num)) {}

		BigNumber& operator=(const BigNumber& another_big) {
			if (this != &another_big) {
				big_num = another_big.big_num;
			}
			return *this;
		}

		BigNumber& operator=(BigNumber&& another_big) noexcept {
			if (this != &another_big) {
				big_num = std::move(another_big.big_num);
			}
			return *this;
		}

		BigNumber operator+(const BigNumber& another_big) const {
			std::vector<int> result;
			int carry = 0;

			int max_size = std::max(big_num.size(), another_big.big_num.size());
			std::vector<int> num1 = big_num;
			std::vector<int> num2 = another_big.big_num;

			while (num1.size() < max_size) num1.insert(num1.begin(), 0);
			while (num2.size() < max_size) num2.insert(num2.begin(), 0);

			for (int i = max_size - 1; i >= 0; i--) {
				int sum = num1[i] + num2[i] + carry;
				carry = sum / 10;
				result.push_back(sum % 10);
			}

			if (carry) {
				result.push_back(carry);
			}

			std::reverse(result.begin(), result.end());
			return BigNumber(result);
		}

		BigNumber operator*(const BigNumber& another_big) const {
			int size1 = big_num.size();
			int size2 = another_big.big_num.size();
			std::vector<int> result(size1 + size2, 0);

			for (int i = size1 - 1; i >= 0; i--) {
				for (int j = size2 - 1; j >= 0; j--) {
					int mul = big_num[i] * another_big.big_num[j];
					int sum = mul + result[i + j + 1];

					result[i + j + 1] = sum % 10;
					result[i + j] += sum / 10;
				}
			}

			while (result.size() > 1 && result[0] == 0) {
				result.erase(result.begin());
			}

			return BigNumber(result);
		}

		BigNumber operator*(int num) const {
			if (num == 0) return BigNumber("0");

			BigNumber result("0");
			BigNumber multiplier = *this;
			int absNum = std::abs(num);

			while (absNum > 0) {
				if (absNum % 2 == 1) {
					result = result + multiplier;
				}
				multiplier = multiplier + multiplier;
				absNum /= 2;
			}

			return result;
		}

		friend std::ostream& operator<<(std::ostream& os, const BigNumber& num) {
			for (int digit : num.big_num) {
				os << digit;
			}
			return os;
		}
	};

	void start() {
		auto number1 = BigNumber("114575");
		auto number2 = BigNumber("78524");

		auto result = number1 + number2;

		std::cout << result << std::endl;

		result = number1 * 2;

		

		std::cout << result << std::endl;

	}
}

int main(int argc, char** argv) {

	//	first_task::start();
	second_task::start();

	return 0;
}