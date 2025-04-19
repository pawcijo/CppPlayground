cmake_minimum_required(VERSION 3.10)
project(MyTests)

set(CMAKE_CXX_STANDARD 20)

# Add GoogleTest submodule
add_subdirectory(googletest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include directories for the project
include_directories(${CMAKE_SOURCE_DIR}/src/Common)
include_directories(${CMAKE_SOURCE_DIR}/src/Demos/CPP17)
include_directories(${CMAKE_SOURCE_DIR}/googletest/googletest/include)

# Add the test executable (ALWAYS REBUILD TO GENERATE)
# Find all source files in the src directory
# and its subdirectories
file(GLOB_RECURSE SOURCES "../src/*.cpp")
add_executable(runTests main_test.cpp ${SOURCES})

# Link GoogleTest to the test executable
target_link_libraries(runTests GTest::gtest GTest::gtest_main)

# Enable testing
enable_testing()
add_test(NAME MyTests COMMAND runTests)

# Add 'check' target to run tests when explicitly called
add_custom_target(check
    COMMAND ctest --output-on-failure --verbose
    DEPENDS runTests
    COMMENT "Running tests"
)
