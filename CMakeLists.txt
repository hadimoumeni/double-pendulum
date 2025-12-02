cmake_minimum_required(VERSION 3.21)
project(DoublePendulumSim C)
set(CMAKE_C_STANDARD 23)

# Find SDL2 (Required for all C projects using it)
find_package(SDL2 REQUIRED)

# Include the 'include' directory globally
include_directories("${PROJECT_SOURCE_DIR}/include")

# Include SDL2 headers
include_directories(${SDL2_INCLUDE_DIRS})

add_subdirectory(src)

# Enable testing (Good for Project Hygiene)
enable_testing()
add_subdirectory(tests) 
# Note: You'll need to define tests here later to meet the 'tests' rubric.