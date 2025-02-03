cmake_minimum_required(VERSION 3.10)
project(MyTests)

set(CMAKE_CXX_STANDARD 20)

# Find and link GTest
find_package(GTest REQUIRED)
message(STATUS "GTEST_INCLUDE_DIRS: ${gtest_SOURCE_DIR}")

# Include directories for the project
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/Common)
include_directories(${CMAKE_SOURCE_DIR}/src/Demos/CPP17)

# Add the test executable
file(GLOB_RECURSE SOURCES "../src/*.cpp")
add_executable(runTests main_test.cpp ${SOURCES})


# Enable testing
enable_testing()
add_test(NAME MyTests COMMAND runTests)

# Add a custom target to run tests after building
add_custom_target(check ALL DEPENDS runTests COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)

target_link_libraries(runTests GTest::gtest GTest::gtest_main)
