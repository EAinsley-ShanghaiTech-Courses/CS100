cmake_minimum_required(VERSION 2.8.12)

project(Rational)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(rational_SRC rational.cpp)
add_library(rational_lib ${rational_SRC})

set(main_SRC main.cpp)
add_executable(main ${main_SRC})

target_link_libraries(main rational_lib)
