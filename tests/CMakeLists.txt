cmake_minimum_required(VERSION 3.14)

include(FetchContent)
FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG        v2.5.2
)
FetchContent_MakeAvailable(unity)

add_executable(run_tests
    test_suite.c
    ../src/arithmetic.c
    ../src/pendulum.c
)

target_link_libraries(run_tests PRIVATE unity m)
target_include_directories(run_tests PRIVATE 
    ../include 
    ${unity_SOURCE_DIR}/src
)

include(GoogleTest)
add_test(NAME AllTests COMMAND run_tests)