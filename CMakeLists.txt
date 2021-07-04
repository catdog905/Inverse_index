# CMakeLists.txt
cmake_minimum_required(VERSION 3.20.3)
project(inverse_index)

find_package(Catch2 REQUIRED)

find_path(CATCH_INCLUDE_DIR catch.hpp)
include_directories(${CATCH_INCLUDE_DIR})

add_executable(inverse_index inverse_index.cpp)
target_link_libraries(inverse_index ${CATCH_INCLUDE_DIR})
