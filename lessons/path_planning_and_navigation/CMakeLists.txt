cmake_minimum_required (VERSION 2.8.3)
project (path_planning_and_navigation)

set(PYTHON_DOT_VERSION 2.7)
set(PYTHON_INCLUDE /usr/include/python2.7)

include_directories(${PYTHON_INCLUDE})

add_executable(minkowski_sum minkowski_sum.cpp)
add_executable(search search.cpp)

add_compile_options(-std=c++11)

target_link_libraries(search -lpython2.7)
