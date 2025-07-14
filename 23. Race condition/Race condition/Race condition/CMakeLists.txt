# CMakeList.txt : CMake project for Race condition, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (CMakeTarget "Race condition.cpp" "allinclude.h" "FirstTask.h" "FirstTask.cpp" "SecondTask.cpp" "SecondTask.h" "ThirdTask.cpp" "ThirdTask.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeTarget PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
