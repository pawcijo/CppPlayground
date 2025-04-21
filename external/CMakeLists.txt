if(MSVC AND NOT "${MSVC_VERSION}" LESS 1400)
	add_definitions( "/MP" )
endif()

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)
### SPRIV-CROSS ###
add_subdirectory (SPIRV-Cross)
### SPIRV-Reflect ##
add_subdirectory (SPIRV-Reflect)

### GLFW ###

add_subdirectory (glfw)

include_directories(
	glfw/include/GLFW/
)
