cmake_minimum_required (VERSION 2.8.3)
project (occupancy_grid_mapping)

set(PYTHON_DOT_VERSION 2.7)
set(PYTHON_INCLUDE /usr/include/python2.7)

include_directories(${PYTHON_INCLUDE})

add_executable(occupancy_grid_mapping occupancy_grid_mapping.cpp)

add_compile_options(-std=c++11)

target_link_libraries(occupancy_grid_mapping -lpython2.7)
