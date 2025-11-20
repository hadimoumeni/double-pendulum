add_executable(test_library test_library.cpp)
target_link_libraries(test_library library GTest::gtest_main)
add_test(NAME test_library COMMAND test_library)