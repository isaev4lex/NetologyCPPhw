cmake_minimum_required(VERSION 3.10)
project(second_task)

set(CMAKE_CXX_STANDART 11)

set(SOURCE_FILES
	lesson_homework_3.cpp
	Figure.cpp
	Triangle.cpp
	RightTriangle.cpp
	IsoscelesTriangle.cpp
	EquilateralTriangle.cpp
	Quadrangle.cpp
	Rectangle.cpp
	Square.cpp
	Parallelogram.cpp
	Romb.cpp
)

set(HEADER_FILES
	Figure.h
	Triangle.h
	RightTriangle.h
	IsoscelesTriangle.h
	EquilateralTriangle.h
	Quadrangle.h
	Rectangle.h
	Square.h
	Parallelogram.h
	Romb.h
)

add_executable(my_second_cmake_program ${SOURCE_FILES} ${HEADER_FILES})
