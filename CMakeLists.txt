project(bhcd)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS_RELEASE "-O2 -g -Wall -Wextra -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-pg -g -Wall -Wextra -std=c++11")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -pg")

add_subdirectory(gtest-1.6.0)
include_directories(gest-1.6.0/include)

add_subdirectory(lib)
include_directories(lib)

add_subdirectory(bin)
add_subdirectory(test)
