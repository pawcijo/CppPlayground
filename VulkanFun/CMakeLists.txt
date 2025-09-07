# CMake entry point
cmake_minimum_required (VERSION 3.10)
project (VulkanFun)

set(CMAKE_CXX_STANDARD 23)

find_package(Vulkan REQUIRED)
find_package(SDL2 REQUIRED)

set(CMAKE_LIBRARY_PATH ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_DEBUG_POSTFIX d)

add_subdirectory (external)

include_directories(
	src/
	external/imgui/
	external/imgui/backends/
	external/glfw/include/
	external/glm/
	external/tinyxml2/
	${SDL2_INCLUDE_DIRS}
	${Vulkan_INCLUDE_DIRS}
	.
)

if(APPLE)
set(ALL_LIBS
	${OPENGL_LIBRARY}
	Vulkan::Vulkan
	glfw
	${SDL2_LIBRARIES}
	tinyxml2
	pthread
)
else()
set(ALL_LIBS
	${OPENGL_LIBRARY}
	Vulkan::Vulkan
	glfw
	${SDL2_LIBRARIES}
	tinyxml2
	pthread
)
endif()


add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-D_CRT_SECURE_NO_WARNINGS
)

set(IMGUI_DIR external/imgui/)


add_executable(${PROJECT_NAME} 
main.cpp
	src/Vulkan/VulkanPipeLine.cpp
	src/Vulkan/VulkanApp.cpp
	src/Vulkan/VulkanShader.cpp
	src/Vulkan/VulkanValidation.hpp
	src/Vulkan/VulkanUtis.cpp

	src/tiny_obj_loader.h

	src/Common/Transform.cpp
	src/Common/Camera.cpp
	
	src/Common/Scene.cpp
	src/Common/SceneUtils.cpp
	src/Common/SceneObject.cpp
	src/Vulkan/Model.cpp
	src/MyPhysics/PhysicsTimer.cpp

	${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
	${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp 
	${IMGUI_DIR}/imgui.cpp 
	${IMGUI_DIR}/imgui_draw.cpp 
	${IMGUI_DIR}/imgui_demo.cpp 
	${IMGUI_DIR}/imgui_tables.cpp 
	${IMGUI_DIR}/imgui_widgets.cpp
)

target_link_libraries(${PROJECT_NAME}
	${ALL_LIBS}
)