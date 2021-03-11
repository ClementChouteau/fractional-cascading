cmake_minimum_required(VERSION 3.17)
project(Fractional_Cascading)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(GTest REQUIRED)
find_package(Benchmark REQUIRED)

include_directories(${Benchmark_INCLUDE_DIRS})

if (Benchmark_FOUND)
  add_subdirectory(benchmark)
endif()
if (GTest_FOUND)
  add_subdirectory(test)
endif()
