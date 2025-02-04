#include <iostream>
#include <cstring>
#include <string>


class MyString {
private:
	char* anystring = nullptr;
public:
	MyString() {
		anystring = new char[1];
		anystring[0] = 0;
	}

	MyString(const char* argstring) {
		anystring = new char[strlen(argstring) + 1];
		strcpy(anystring, argstring);
	}

	void operator=(const char* argstring) {
		delete[] anystring;
		anystring = new char[strlen(argstring) + 1];
		strcpy(anystring, argstring);
	}
	
	void operator=(const MyString& mystring) {
		delete[] anystring;
		anystring = new char[strlen(mystring.anystring) + 1];
		strcpy(anystring, mystring.anystring);
	}
	
	void operator=(MyString&& mystring) {
		delete[] anystring;
		anystring = mystring.anystring;
		mystring.anystring = nullptr;
	}

	MyString() {
		delete[] anystring;
	}
};


int main(int argc, char** argv) {
	MyString a;
	MyString b;
	b = MyString("Hello world");

	a = std::move(b);

	return 0;
}