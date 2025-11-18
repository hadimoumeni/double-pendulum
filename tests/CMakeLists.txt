add_executable(test_library test_library.c)
add_test(NAME test_library COMMAND test_library)
target_link_libraries(test_library gtest gtest_main library)