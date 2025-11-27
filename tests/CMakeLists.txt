# add_executable(test_pendulum test_pendulum.c)
# target_link_libraries(test_pendulum PRIVATE library)
# target_include_directories(test_library PRIVATE ${PROJECT_SOURCE_DIR}/third_party)

# add_test(NAME test_pendulum COMMAND test_pendulum)

add_executable(arithmetics_test arithmetics_test.c)
target_link_libraries(arithmetics_test PRIVATE arithmetic m)

add_test(NAME arithmetics_test COMMAND arithmetics_test)
