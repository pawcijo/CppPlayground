cmake_minimum_required(VERSION 3.10)

# Project name and version
project(PlaygroundLib VERSION 1.0)

# Use modern C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add extra warnings and treat warnings as errors
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# Collect source files
file(GLOB_RECURSE SOURCES
    "Common/*.cpp"
    "Demos/*.cpp"
    "DemosFactory/*.cpp"
)

message(STATUS "Building static library from: ${SOURCES}")

# Build static library
add_library(PlaygroundLib STATIC ${SOURCES})

#target_compile_definitions(PlaygroundLib PUBLIC CONSOLE_LOG_FORMATTER=1)

target_include_directories(PlaygroundLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
