cmake_minimum_required(VERSION 3.21)
project(example-project LANGUAGES C CXX)

include_directories(include)
add_subdirectory(src)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_subdirectory(tests)