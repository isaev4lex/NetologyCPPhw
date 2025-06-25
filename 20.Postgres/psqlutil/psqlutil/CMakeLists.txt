cmake_minimum_required(VERSION 3.15)
project(psqlUtil)

add_executable(psqlUtil "psqlUtil.cpp" "psqlUtil.h")

target_include_directories(psqlUtil PRIVATE 
    "C:/Users/Saint Alex/Desktop/Netology HW/20.Postgres/psqlutil/libpqxx/installed/include"
    "D:/Programs/PostgreSQL/17/include"
)

link_directories(
    "C:/Users/Saint Alex/Desktop/Netology HW/20.Postgres/psqlutil/libpqxx/installed/lib"
    "D:/Programs/PostgreSQL/17/lib"
)

target_link_libraries(psqlUtil PRIVATE
    "C:/Users/Saint Alex/Desktop/Netology HW/20.Postgres/psqlutil/libpqxx/installed/lib/pqxx.lib"
    "D:/Programs/PostgreSQL/17/lib/libpq.lib"
    Ws2_32
)

set_property(TARGET psqlUtil PROPERTY VS_DEBUGGER_ENVIRONMENT
    "PATH=%PATH%;D:\\Programs\\PostgreSQL\\17\\bin")
