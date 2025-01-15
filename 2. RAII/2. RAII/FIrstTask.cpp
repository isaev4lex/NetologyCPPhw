#include "allinclude.h"
#include "FirstTask.h"

SmartArray::SmartArray(int count) : count(count) {
	smart_array = new int[count];
}

SmartArray::SmartArray(const SmartArray& smart_array_obj) {
	delete[] smart_array;
	this->count = smart_array_obj.count;
	smart_array = new int[smart_array_obj.count];
	for (int i = 0; i < smart_array_obj.count; i++) {
		smart_array[i] = smart_array_obj.smart_array[i];
	};
};

void SmartArray::set_in_array(int index, int value) {
	if (index <= count && index >= 0) {
		smart_array[index] = value;
	}
	else {
		throw std::exception("Index out of range");
	}
}

int SmartArray::get_element(int index) {
	if (index <= count && index >= 0) {
		return smart_array[index];
	} else {
		throw std::exception("Index out of range");
	}
}

SmartArray& SmartArray::operator=(const SmartArray& smart_array_obj) {
	if (this == &smart_array_obj) {
		return *this;
	}
	delete[] smart_array;

	smart_array = new int[smart_array_obj.count];
	for (int i = 0; i < smart_array_obj.count; i++) {
		smart_array[i] = smart_array_obj.smart_array[i];
	}

	return *this;
}

SmartArray::~SmartArray() {
	delete[] smart_array;
}
