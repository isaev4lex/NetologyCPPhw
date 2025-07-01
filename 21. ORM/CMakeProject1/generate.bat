@echo off
cmake -B build -S . ^
  -DCMAKE_TOOLCHAIN_FILE=D:/Programs/Dev/vcpkg/scripts/buildsystems/vcpkg.cmake ^
  -DCMAKE_BUILD_TYPE=Debug

cmake --build build
pause
