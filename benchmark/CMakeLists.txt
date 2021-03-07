add_executable(${PROJECT_NAME}-benchmark benchmark.cpp)
cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)

find_package(Benchmark REQUIRED)

target_include_directories(${PROJECT_NAME}-benchmark
  PUBLIC
  ../include
  PRIVATE
  ${Benchmark_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}-benchmark ${Benchmark_LIBRARIES} pthread)
