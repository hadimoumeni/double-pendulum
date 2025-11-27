cmake_minimum_required(VERSION 3.21)
project(example-project VERSION 1.0 LANGUAGES C)
set(CMAKE_C_STANDARD 23)

add_subdirectory(src)

include_directories(${PROJECT_SOURCE_DIR}/third_party)

enable_testing()
add_subdirectory(tests)
#target_link_libraries(test_pendulum PRIVATE library)
#add_test(test_pendulum test_pendulum)
