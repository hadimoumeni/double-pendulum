cmake_minimum_required(VERSION 3.21)
project(DoublePendulumSim C)
set(CMAKE_C_STANDARD 23)

find_package(SDL2 REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/include")

include_directories(${SDL2_INCLUDE_DIRS})


add_subdirectory(src)


#add_subdirectory(tests)

#include_directories(${PROJECT_SOURCE_DIR}/third_party)

#(UNCOMMENT TO ENABLE TESTING)

#enable_testing()
#target_link_libraries(test_pendulum PRIVATE library)
#add_test(test_pendulum test_pendulum)
