#include <iostream>
#include <string>
#include "Catch2/catch_session.hpp"
#include "Catch2/catch_test_macros.hpp"
#include "first.h"


//////////////////////////// FIRST TASK ///////////////////////////////

// Checing for 'Empty' methond
TEST_CASE("CASE for empty 1", "Creating new and checking if empty") {
	List list;
	REQUIRE(list.Empty());
}

TEST_CASE("CASE for empty 2", "Creating new and adding one and checking if empty") {
	List list;
	list.PushFront(324);
	REQUIRE(!list.Empty());
}

TEST_CASE("CASE for empty 3", "Creating new and adding one, deleting everything and checking if empty") {
	List list;
	list.PushFront(324);
	list.Clear();
	REQUIRE(list.Empty());
}


// Checking for 'Size' method
TEST_CASE("Case for size 1", "Creating empty and check if size == 0") {
	List list;
	REQUIRE(list.Size() == 0);
}
TEST_CASE("Case for size 2", "Creating empty and adding 4 elem and check if size == 4") {
	List list;
	list.PushFront(324);
	list.PushFront(322);
	list.PushFront(32321);
	list.PushFront(3);
	REQUIRE(list.Size() == 4);
}
TEST_CASE("Case for size 3", "Creating empty and adding 4 elem and clearing and check if size == 0") {
	List list;
	list.PushFront(324);
	list.PushFront(322);
	list.PushFront(32321);
	list.PushFront(3);
	list.Clear();
	REQUIRE(list.Size() == 0);
}

// Checking for 'Clear' method
TEST_CASE("Case for Clear 1", "Creating empty and adding 1 elem clearing and checking size") {
	List list;
	list.PushFront(3231);
	list.Clear();
	REQUIRE(list.Size() == 0);
}
TEST_CASE("Case for Clear 2", "Creating empty and adding 1 elem clearing and checking size and doing that again") {
	List list;
	list.PushFront(3231);
	list.Clear();
	list.PushFront(3231);
	list.Clear();
	REQUIRE(list.Size() == 0);
}



//////////////////////////// SECOND TASK ///////////////////////////////
TEST_CASE("Case for PushBack 1", "Adding to start and checking if was added.") {
	List list;
	list.PushBack(3231);
	REQUIRE(list.PopBack() == 3231);
}

TEST_CASE("Case for PushBack 2", "Adding to start 2 times and checking if was added.") {
	List list;
	list.PushBack(3231);
	list.PushBack(123);

	bool result = list.PopBack() == 123 && list.PopFront() == 3231;

	REQUIRE(result);
}
TEST_CASE("Case for PushFront 1", "Adding to end and checking if was added.") {
	List list;
	list.PushFront(3231);
	REQUIRE(list.PopFront() == 3231);
}

TEST_CASE("Case for PushFront 2", "Adding to end 2 times and checking if was added.") {
	List list;
	list.PushFront(3231);
	list.PushFront(123);

	bool result = list.PopBack() == 3231 && list.PopFront() == 123;

	REQUIRE(result);
}
TEST_CASE("Case for PopBack 1", "Creating empty, checking if we have exception") {
	List list;
	REQUIRE_THROWS_AS(list.PopBack(), std::runtime_error);
}
TEST_CASE("Case for PopBack 2", "Creating empty, adding one and checking if we DON'T have exception") {
	List list;
	list.PushFront(324);
	REQUIRE_NOTHROW(list.PopFront());
}
TEST_CASE("Case for PopFront 1", "Creating empty, checking if we have exception") {
	List list;
	REQUIRE_THROWS_AS(list.PopFront(), std::runtime_error);
}
TEST_CASE("Case for PopFront 2", "Creating empty, adding one and checking if we DON'T have exception") {
	List list;
	list.PushFront(324);
	REQUIRE_NOTHROW(list.PopFront());
}



int main(int argv, char** argc) {
	return Catch::Session().run(argv, argc);
}