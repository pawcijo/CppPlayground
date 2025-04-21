cmake_minimum_required(VERSION 3.10)

# Project name and version
project(ModernCpp VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add extra warnings and treat warnings as errors
#TODO set this for static lib  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic ")
# Debug flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")


add_subdirectory (external)

# add include directories
include_directories(
	VulkanSrc/Vulkan
	src/
	external/imgui/
	external/glm/
	external/imgui/backends/
	external/glfw/include/
	${Vulkan_INCLUDE_DIRS}
	.
)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

set(ALL_LIBS
	Vulkan::Vulkan
	glfw
	pthread
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

set(IMGUI_DIR external/ImGui/)

# Add the executable
add_executable(ModernCpp 
    main.cpp
	VulkanSrc/Vulkan/VulkanPipeline.cpp
	VulkanSrc/Vulkan/VulkanApp.cpp
	VulkanSrc/Vulkan/VulkanShader.cpp
	VulkanSrc/Vulkan/VulkanValidation.hpp
	${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
	${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp 
	${IMGUI_DIR}/imgui.cpp 
	${IMGUI_DIR}/imgui_draw.cpp 
	${IMGUI_DIR}/imgui_demo.cpp 
	${IMGUI_DIR}/imgui_tables.cpp 
	${IMGUI_DIR}/imgui_widgets.cpp
    ${SOURCES}
)


target_link_libraries(ModernCpp ${ALL_LIBS})