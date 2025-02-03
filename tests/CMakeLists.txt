cmake_minimum_required(VERSION 3.10)
project(MyTests)

set(CMAKE_CXX_STANDARD 20)

# Add GoogleTest submodule
add_subdirectory(googletest)

# Include directories for the project
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

# Link GoogleTest to your test executable
target_link_libraries(runTests gtest gtest_main)
