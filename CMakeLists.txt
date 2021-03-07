cmake_minimum_required(VERSION 3.17)
project(Fractional_Cascading)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include_directories(${Benchmark_INCLUDE_DIRS})

add_subdirectory(benchmark)
add_subdirectory(test)
