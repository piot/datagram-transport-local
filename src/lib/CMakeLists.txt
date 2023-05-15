cmake_minimum_required(VERSION 3.17)
project(datagram_transport_local C)

set(CMAKE_C_STANDARD 99)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(isDebug TRUE)
else()
  message("no debug")
  set(isDebug FALSE)
endif()

if(isDebug)
  message("Debug build detected")
  add_compile_definitions(CONFIGURATION_DEBUG TYRAN_CONFIGURATION_DEBUG)
  set(CONFIGURATION_DEBUG 1)
endif()

set(deps ../../deps/)

file(GLOB_RECURSE deps_src FOLLOW_SYMLINKS "${deps}*/src/lib/*.c")

file(GLOB_RECURSE lib_src FOLLOW_SYMLINKS "*.c")

add_library(datagram_transport_local ${deps_src} ${lib_src})

include_directories(../include)
target_include_directories(datagram_transport_local
                           PUBLIC ${deps}/piot/discoid-c/src/include)
target_include_directories(datagram_transport_local
                           PUBLIC ${deps}/piot/monotonic-time-c/src/include)
target_include_directories(datagram_transport_local
                           PUBLIC ${deps}/piot/datagram-transport-c/src/include)
target_include_directories(datagram_transport_local
                           PUBLIC ${deps}/piot/clog/src/include)
target_include_directories(datagram_transport_local
                           PUBLIC ${deps}/piot/tiny-libc/src/include)

target_link_libraries(datagram_transport_local)
