add_executable(app app.cpp)

target_link_libraries(app PRIVATE test_library)
target_link_libraries(app PRIVATE matrix_library)

