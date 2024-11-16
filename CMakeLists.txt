cmake_minimum_required(VERSION 3.10)

project(assignment VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 
)

FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)


add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(tests)
