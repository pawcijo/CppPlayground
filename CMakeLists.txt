cmake_minimum_required(VERSION 3.10)

# Project name and version
project(ModernCpp VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler settings for Xcode (only on macOS)
if(APPLE)
 set(CMAKE_C_COMPILER "/usr/bin/clang")
 set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
endif()


add_subdirectory(PlaygroundLib)
add_subdirectory(tests)
add_subdirectory(ModernCppQTApp)
# add # to disable VulkanFun build
add_subdirectory(VulkanFun)

