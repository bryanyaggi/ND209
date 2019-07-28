cmake_minimum_required (VERSION 2.8.3)
project (monte_carlo_localization)

set(PYTHON_DOT_VERSION 2.7)
set(PYTHON_INCLUDE /usr/include/python2.7)

include_directories(${PYTHON_INCLUDE})

add_executable(bayes_practice bayes_practice.cpp)
add_executable(robot_mcl robot_mcl.cpp)

add_compile_options(-std=c++11)

target_link_libraries(robot_mcl -lpython2.7)
