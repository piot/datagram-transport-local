cmake_minimum_required(VERSION 3.17)
project(datagram_transport_local C)

set(CMAKE_C_STANDARD 99)


if (WIN32)
    add_compile_definitions(TORNADO_OS_WINDOWS 1)
endif(WIN32)

add_subdirectory("lib")
add_subdirectory("test")
