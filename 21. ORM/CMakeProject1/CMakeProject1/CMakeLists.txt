cmake_minimum_required(VERSION 3.16)
project(CMakeProject1)

set(CMAKE_CXX_STANDARD 20)

find_package(wt CONFIG REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(libpqxx CONFIG REQUIRED)

add_executable(CMakeProject1
  "CMakeProject1.cpp"
  "models.cpp"
  "db.cpp"
)

target_link_libraries(CMakeProject1 PRIVATE
  Wt::Wt
  Wt::Dbo
  Wt::DboPostgres
  PostgreSQL::PostgreSQL
  libpqxx::pqxx
)
