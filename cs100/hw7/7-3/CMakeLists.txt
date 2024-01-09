cmake_minimum_required(VERSION 2.8.12)

project(KMeans)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE "REALEASE")

set(kmeans_SRC kmeans.cpp)
add_library(kmeans_lib ${kmeans_SRC})

set(main_SRC main.cpp)
add_executable(main ${main_SRC})

target_link_libraries(main kmeans_lib)
