cmake_minimum_required(VERSION 3.10) # Specify the minimum version for CMake

# Project name and version
project(ModernCpp VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

include_directories(
	src/
    .
)

file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add the executable
add_executable(ModernCpp 
                main.cpp
                ${SOURCES}
                )

