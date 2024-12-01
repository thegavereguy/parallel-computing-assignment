cmake_minimum_required(VERSION 3.10)

project(assignment VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_OPTIMIZED_O1 "Build with -O1" OFF)
option(BUILD_OPTIMIZED_O2 "Build with -O2" OFF)
option(BUILD_OPTIMIZED_VEC "Build with vectorization" OFF)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 
)

FetchContent_Declare(
	perf-cpp-external
	GIT_REPOSITORY https://github.com/jmuehlig/perf-cpp.git
	GIT_TAG        v0.8
)

FetchContent_MakeAvailable(Catch2)
FetchContent_MakeAvailable(perf-cpp-external)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

include_directories(${perf-cpp-external_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(tests)

