add_library(test_library lib.cpp ../include/lib/lib.h)
add_library(matrix_library matrix.cpp ../include/lib/matrix.h)
add_library(tools_library tools.cpp ../include/lib/tools.h)

target_include_directories(test_library PUBLIC ../include)
target_include_directories(matrix_library PUBLIC ../include)
target_include_directories(tools_library PUBLIC ../include)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(test_library PUBLIC OpenMP::OpenMP_CXX)
    target_link_libraries(matrix_library PUBLIC OpenMP::OpenMP_CXX)
endif()
