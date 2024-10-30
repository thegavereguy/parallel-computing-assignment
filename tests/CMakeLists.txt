add_executable(tests test.cpp)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE test_library)

catch_discover_tests(tests)
