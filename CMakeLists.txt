cmake_minimum_required(VERSION 3.21)
project(DoublePendulumSim C)
add_definitions(-DUNITY_INCLUDE_DOUBLE)
set(CMAKE_C_STANDARD 23)

find_package(SDL2 REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/include")

include_directories(${SDL2_INCLUDE_DIRS})

add_subdirectory(src)

enable_testing()
add_subdirectory(tests) 
