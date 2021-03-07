add_executable(${PROJECT_NAME}-test test.cpp)
cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME}-test
  PUBLIC
  ../include
)

target_link_libraries(${PROJECT_NAME}-test
  GTest::GTest GTest::Main
)

add_test(FractionalCascadingTest ${PROJECT_NAME}-test)
