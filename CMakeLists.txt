cmake_minimum_required(VERSION 3.21)
project(DoublePendulumSim C)
set(CMAKE_C_STANDARD 11)

# 1. Find SDL2 (Required for the graphics)
find_package(SDL2 REQUIRED)

# 2. Global Include Directories
include_directories("${PROJECT_SOURCE_DIR}/include")
include_directories(${SDL2_INCLUDE_DIRS})

# 3. Add the Source Code Directory
add_subdirectory(src)

# 4. Add the Tests Directory (Project Hygiene)
enable_testing()
add_subdirectory(tests)