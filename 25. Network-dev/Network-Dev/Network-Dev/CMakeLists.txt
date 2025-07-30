# CMakeList.txt : CMake project for Network-Dev, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (Network-Dev "main.cpp"  "allinclude.h" "UDPclient.cpp" "UDPserver.cpp" "UDPserver.h" "UDPclient.h" "TCPserver.h" "TCPserver.cpp" "TCPclient.h" "TCPclient.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Network-Dev PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
