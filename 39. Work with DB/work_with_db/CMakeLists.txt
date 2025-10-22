cmake_minimum_required(VERSION 3.16)

project(work_with_db VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# === ВАЖНО: добавляем Qt Sql ===
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(work_with_db
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(work_with_db SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(work_with_db
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# === Линкуем виджеты + SQL ===
target_link_libraries(work_with_db PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# (Опционально, если у преподавателя папка PG_Libs в проекте)
# set(PG_LIBS_DIR "${CMAKE_SOURCE_DIR}/PG_Libs")
# if(WIN32 AND EXISTS "${PG_LIBS_DIR}")
#     target_link_directories(work_with_db PRIVATE "${PG_LIBS_DIR}")
#     # Например, если там лежат libpq.lib и пр. — раскомментируй под свои имена:
#     # target_link_libraries(work_with_db PRIVATE libpq)
#     # На Windows не забудь положить DLL'ки (libpq.dll, libcrypto-*.dll и т.п.) рядом с exe
# endif()

# Бандл-метаданные
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.work_with_db)
endif()
set_target_properties(work_with_db PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS work_with_db
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(work_with_db)
endif()
