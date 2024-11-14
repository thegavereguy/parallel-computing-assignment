add_executable(tests test.cpp)
add_executable(benchmarks benchmarks.cpp)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE matrix_library)
target_link_libraries(tests PRIVATE tools_library)
target_link_libraries(benchmarks PRIVATE Catch2::Catch2WithMain)
target_link_libraries(benchmarks PRIVATE matrix_library)
target_link_libraries(benchmarks PRIVATE tools_library)

catch_discover_tests(tests)
catch_discover_tests(benchmarks)
