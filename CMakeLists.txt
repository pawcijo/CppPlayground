cmake_minimum_required(VERSION 3.10)

# Project name and version
project(ModernCpp VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(PlaygroundLib)
add_subdirectory(tests)
add_subdirectory(ModernCppQTApp)
# add # to disable GRPCDemo build
add_subdirectory(grpcDemo)
# add # to disable VulkanFun build
add_subdirectory(VulkanFun)

