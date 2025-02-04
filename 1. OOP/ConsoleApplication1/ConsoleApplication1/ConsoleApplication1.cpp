#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Address {
private:
    std::string city;
    std::string street;
    int house_number;
    int apartment_number;

public:
    Address(const std::string& city = "", const std::string& street = "", int house = 0, int apartment = 0)
        : city(city), street(street), house_number(house), apartment_number(apartment) {}

    std::string get_output_address() const {
        std::ostringstream oss;
        oss << city << ", " << street << ", дом " << house_number << ", кв. " << apartment_number;
        return oss.str();
    }
};

class FileWorker {
private:
    std::string in_file;
    std::string out_file;
    int addressed_count;
    int rows_count_until_new_address;

    Address* read_addresses() {
        std::ifstream in(in_file);
        if (!in.is_open()) {
            std::cerr << "Ошибка открытия файла.\n";
            return nullptr;
        }

        in >> addressed_count;
        Address* addresses = new Address[addressed_count];

        for (int i = 0; i < addressed_count; ++i) {
            std::string city, street;
            int house, apartment;

            in >> city >> street >> house >> apartment;
            addresses[i] = Address(city, street, house, apartment);
        }
        return addresses;
    }

    void save_addresses(Address* addresses, bool reversed, bool alphabet_sort) {
        std::ofstream out(out_file);
        if (!out.is_open()) {
            std::cerr << "Ошибка открытия файла для записи.\n";
            return;
        }

        if (alphabet_sort) {
            for (int i = 0; i < addressed_count - 1; ++i) {
                for (int j = i + 1; j < addressed_count; ++j) {
                    if (addresses[i].get_output_address() > addresses[j].get_output_address()) {
                        Address temp = addresses[i];
                        addresses[i] = addresses[j];
                        addresses[j] = temp;
                    }
                }
            }
        }

        out << addressed_count << "\n";
        if (reversed) {
            for (int i = addressed_count - 1; i >= 0; --i) {
                out << addresses[i].get_output_address() << "\n";
            }
        }
        else {
            for (int i = 0; i < addressed_count; ++i) {
                out << addresses[i].get_output_address() << "\n";
            }
        }
    }

public:
    FileWorker(std::string in_file = "in.txt", std::string out_file = "out.txt",
        int rows_count_until_new_address = 4)
        : in_file(in_file), out_file(out_file), rows_count_until_new_address(rows_count_until_new_address) {}

    void sort_to_file(bool reversed = false, bool alphabet_sort = false) {
        Address* addresses = read_addresses();
        if (addresses) {
            save_addresses(addresses, reversed, alphabet_sort);
            delete[] addresses;
        }
    }
};

int main() {
    FileWorker fw;
    fw.sort_to_file(true, true);
}
