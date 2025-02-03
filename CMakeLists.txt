cmake_minimum_required(VERSION 3.10)

# Project name and version
project(ModernCpp VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add extra warnings and treat warnings as errors
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")

# Debug flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# add include directories
include_directories(
    src/
    .
)

# ----------------------------
# Build Main Application
# ----------------------------

# ! ! ! !
# Remember to clear cache if recursive glob does not work,
# it will not pick up new files sometimes.

file(GLOB_RECURSE SOURCES "src/*.cpp")
message(STATUS "Found source files: ${SOURCES}")

# Add the executable
add_executable(ModernCpp 
    main.cpp
    ${SOURCES}
)

# ----------------------------
# GoogleTest Integration (GTest)
# ----------------------------
include(FetchContent)

# Fetch GoogleTest if not already installed
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1 # Use latest stable version
)
FetchContent_MakeAvailable(googletest)

# Enable testing in CMake
enable_testing()

# GoogleTest include directories
set(GTEST_INCLUDE_DIRS ${googletest_SOURCE_DIR}/include)
include_directories(${GTEST_INCLUDE_DIRS})

# ----------------------------
# Build Unit Tests
# ----------------------------

# ! ! ! !
# Remember to clear cache if recursive glob does not work,
# it will not pick up new test files sometimes.

file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
message(STATUS "Found test files: ${TEST_SOURCES}")

# Create the test executable
add_executable(ModernCppTests ${TEST_SOURCES})

# Link test executable with GoogleTest
target_link_libraries(ModernCppTests 
    PRIVATE
    gtest
    gtest_main
    pthread  # Required for Linux/macOS
)

# Register tests with CTest
add_test(NAME RunTests COMMAND ModernCppTests)
