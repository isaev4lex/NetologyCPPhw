#pragma once
class SmartArray {
private:
	int count;
	int* smart_array;
public:
	SmartArray(int count);
	SmartArray(const SmartArray& smart_array_obj);

	void set_in_array(int index, int value);

	int get_element(int index);

	SmartArray& operator=(const SmartArray& smart_array_obj);

	~SmartArray();
};