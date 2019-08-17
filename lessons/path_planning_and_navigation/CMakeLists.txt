cmake_minimum_required (VERSION 2.8.3)
project (path_planning_and_navigation)

add_executable(minkowski_sum minkowski_sum.cpp)
add_executable(bfs bfs.cpp)

add_compile_options(-std=c++11)
