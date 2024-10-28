add_library(test_library lib.cpp ../include/lib/lib.h)

target_include_directories(test_library PUBLIC ../include)
