cmake_minimum_required(VERSION 3.16)
project(CMakeProject1)

add_executable(CMakeProject1
  "CMakeProject1.cpp"
  "models.cpp"
  "db.cpp"
)

set_property(TARGET CMakeProject1 PROPERTY CXX_STANDARD 20)

# Include dirs
target_include_directories(CMakeProject1 PRIVATE
  "D:/Programs/Dev/vcpkg/installed/x64-windows/include"
  "D:/Programs/PostgreSQL/17/include"
)

# Прямые пути к .lib
target_link_libraries(CMakeProject1 PRIVATE
  "D:/Programs/Dev/vcpkg/installed/x64-windows/lib/wtdbo.lib"
  "D:/Programs/Dev/vcpkg/installed/x64-windows/lib/wtdbopostgres.lib"
  "D:/Programs/Dev/vcpkg/installed/x64-windows/lib/wt.lib"
  "D:/Programs/PostgreSQL/17/lib/libpq.lib"
  ws2_32
)
