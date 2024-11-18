add_executable(app app.cpp)
add_executable(cache cache.cpp)

target_link_libraries(app PRIVATE matrix_library)
target_link_libraries(app PRIVATE tools_library)
target_link_libraries(app PRIVATE perf-cpp)
target_link_libraries(cache PRIVATE matrix_library)
target_link_libraries(cache PRIVATE tools_library)
target_link_libraries(cache PRIVATE perf-cpp)

