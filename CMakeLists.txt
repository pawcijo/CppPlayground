cmake_minimum_required(VERSION 3.10)

# Project name and version
project(ModernCpp VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

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

add_subdirectory(tests)

# Add the executable
add_executable(ModernCpp 
    main.cpp
    ${SOURCES}
)
