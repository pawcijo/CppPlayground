cmake_minimum_required(VERSION 3.10)
project(MyTests)

set(CMAKE_CXX_STANDARD 20)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runTests main_test.cpp)

find_package(GTest REQUIRED)
target_link_libraries(runTests GTest::GTest GTest::Main)