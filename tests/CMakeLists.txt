cmake_minimum_required(VERSION 3.10)
project(MyTests)

set(CMAKE_CXX_STANDARD 23)

# Add GoogleTest submodule
add_subdirectory(googletest)


set_property(TARGET gtest PROPERTY CXX_STANDARD 23)
set_property(TARGET gtest_main PROPERTY CXX_STANDARD 23)
set_property(TARGET gtest PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET gtest_main PROPERTY CXX_STANDARD_REQUIRED ON)

target_compile_options(gtest PUBLIC -stdlib=libc++)
target_compile_options(gtest_main PUBLIC -stdlib=libc++)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include directories for the project
# Using target_include_directories for better scoping
include_directories(
    ${CMAKE_SOURCE_DIR}/src/Common
    ${CMAKE_SOURCE_DIR}/src/Demos/CPP17
    ${CMAKE_SOURCE_DIR}/googletest/googletest/include
)

# Collect all source files from the src directory
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Add the test executable
add_executable(runTests main_test.cpp ${SOURCES})

# For runTests
target_compile_options(runTests PRIVATE
    -std=c++23       # ensure C++23
    -stdlib=libc++   # use LLVM libc++ headers
    -Wall -Wextra -pedantic -Werror
    $<$<CONFIG:Debug>:-g -O0>
)


# Link GoogleTest and PlaygroundLib (ensure that PlaygroundLib is built first)
add_dependencies(runTests PlaygroundLib)

# Link the necessary libraries (PlaygroundLib, GoogleTest)
target_link_libraries(runTests 
    PlaygroundLib 
    GTest::gtest 
    GTest::gtest_main
    c++         # libc++ runtime
    c++abi      # libc++abi runtime
)

# Enable testing
enable_testing()
add_test(NAME MyTests COMMAND runTests)

# Add 'check' target to run tests when explicitly called
add_custom_target(check
    COMMAND ctest --output-on-failure --verbose
    DEPENDS runTests
    COMMENT "Running tests"
)
