cmake_minimum_required(VERSION 3.16)
project(untitled VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    dbmanager.cpp
    dbmanager.h
    bookingdialog.cpp
    bookingdialog.h
    bookingdialog.ui
    bookingsdialog.cpp
    bookingsdialog.h
    bookingsdialog.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(untitled MANUAL_FINALIZATION ${PROJECT_SOURCES})
else()
  add_executable(untitled ${PROJECT_SOURCES})
endif()

target_link_libraries(untitled PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

set_target_properties(untitled PROPERTIES MACOSX_BUNDLE TRUE WIN32_EXECUTABLE TRUE)
if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(untitled)
endif()
