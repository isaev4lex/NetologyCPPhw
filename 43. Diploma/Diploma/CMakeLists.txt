cmake_minimum_required(VERSION 3.16)

project(diploma VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Sql Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql Widgets)

# ---------- общая библиотека ----------
add_library(search_shared STATIC
  config.cpp config.h
  db.cpp db.h
  textnormalizer.cpp textnormalizer.h
  hasher.cpp hasher.h
)
target_link_libraries(search_shared PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Sql)

# ---------- GUI: searcher ----------
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(searcher
    MANUAL_FINALIZATION
    main.cpp
    mainwindow.cpp mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(searcher
    main.cpp
    mainwindow.cpp mainwindow.h
    mainwindow.ui
  )
endif()
target_link_libraries(searcher PRIVATE search_shared Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Sql Qt${QT_VERSION_MAJOR}::Widgets)
set_target_properties(searcher PROPERTIES
  MACOSX_BUNDLE TRUE
  WIN32_EXECUTABLE TRUE
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# ---------- Консоль: indexer ----------
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(indexer
    MANUAL_FINALIZATION
    indexer.cpp
  )
else()
  add_executable(indexer
    indexer.cpp
  )
endif()
target_link_libraries(indexer PRIVATE search_shared Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Sql)
set_target_properties(indexer PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# (Qt6) финализация
if(QT_VERSION_MAJOR EQUAL 6)
  qt_finalize_executable(searcher)
  qt_finalize_executable(indexer)
endif()
