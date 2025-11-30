cmake_minimum_required(VERSION 3.14)

# 1. Fetch Unity Testing Framework
include(FetchContent)
FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG        v2.5.2
)
FetchContent_MakeAvailable(unity)

# 2. Define the Test Executable
# Note: We do NOT include main.c or sdl_visuals.c here
add_executable(run_tests
    test_suite.c
    ../src/arithmetic.c
    ../src/pendulum.c
)

# 3. Link Libraries
target_link_libraries(run_tests PRIVATE unity m)
target_include_directories(run_tests PRIVATE 
    ../include 
    ${unity_SOURCE_DIR}/src
)

# 4. Register with CTest
include(GoogleTest)
add_test(NAME AllTests COMMAND run_tests)