cmake_minimum_required (VERSION 2.8.12.2)
project(cpp-util)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif(NOT CMAKE_BUILD_TYPE)

set (CXX_COMMON_FLAGS "-std=c++14 -Wall -Wextra")
set (CMAKE_CXX_FLAGS_DEBUG "${CXX_COMMON_FLAGS} -O0 -g")
set (CMAKE_CXX_FLAGS_RELEASE "${CXX_COMMON_FLAGS} -O2")

find_package(Boost COMPONENTS regex system thread unit_test_framework serialization program_options REQUIRED)

add_subdirectory (lib)
add_subdirectory (ut)
