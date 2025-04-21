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

add_subdirectory(PlaygroundLib)

include_directories(ModernCpp PUBLIC
	PlayGroundLib/
)

add_subdirectory(tests)

# Add the executable
add_executable(ModernCpp 
    main.cpp
)

# Link the static library
target_link_libraries(ModernCpp
    PlaygroundLib
)
