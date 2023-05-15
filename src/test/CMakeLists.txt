cmake_minimum_required(VERSION 3.17)
project(datagram_transport_local C)
enable_testing()

set(CMAKE_C_STANDARD 99)

set(deps ../../deps/)
set(local_deps ./deps/)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(isDebug TRUE)
else()
  set(isDebug FALSE)
endif()

if(isDebug)
  message("Debug build detected")
  add_compile_definitions(CONFIGURATION_DEBUG TYRAN_CONFIGURATION_DEBUG)
  set(CONFIGURATION_DEBUG 1)
endif()

file(GLOB_RECURSE local_deps_src FOLLOW_SYMLINKS "${local_deps}*/src/lib/*.c"
     "${deps}*/*/src/platform/posix/*.c" "${deps}*/*/src/lib/*.c")

add_executable(datagram_transport_local_test main.c test.c ${local_deps_src})

add_test(NAME datagram_transport_local_test
         COMMAND datagram_transport_local_test)

target_include_directories(datagram_transport_local_test PRIVATE ../include)
target_include_directories(datagram_transport_local_test
                           PUBLIC ${deps}/piot/imprint/src/include)

if(WIN32)
  target_link_libraries(datagram_transport_local_test datagram_transport_local)
else()
  target_link_libraries(datagram_transport_local_test datagram_transport_local
                        m)
endif(WIN32)
