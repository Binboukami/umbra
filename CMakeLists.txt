cmake_minimum_required(VERSION 3.1)

include(FetchContent)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_FLAGS "-Wno-attributes -fPIC")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set (CMAKE_CXX_FLAGS "-fpermissive -fPIC")

project(
		Umbra
		VERSION 0.1.0
		LANGUAGES C CXX
)

#region GLFW
FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.3.9 # e2c92645460f680fd272fd2eed591efb2be7dc31
	# OVERRIDE_FIND_PACKAGE -- This line broke on unix
)

FetchContent_MakeAvailable(glfw)

set(GLFW_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Comment line below so it builds on unix environment. Review it later
# find_package(GLFW REQUIRED)
#endregion GLFW

#region GLAD
set(GLAD_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/glad)

add_library(
	glad
	STATIC
	${GLAD_LIB_PATH}/src/glad.c)

include_directories(${GLAD_LIB_PATH}/include)
#endregion GLAD

#region Freetype
# TODO: Add unix dynamic libraries
file(COPY lib/freetype/freetype.dll DESTINATION ${CMAKE_BINARY_DIR})
file(COPY lib/freetype/freetype.lib DESTINATION ${CMAKE_BINARY_DIR}/static)
file(COPY lib/freetype/freetype.lib DESTINATION ${CMAKE_BINARY_DIR})
include_directories(include/freetype)
#endregion

include_directories(include/umbra)
include_directories(lib/stb)

add_library(
	${PROJECT_NAME}

	SHARED

		lib/stb/stb_image.cpp

		include/umbra/file_system.h
		include/umbra/export_macros.h
		include/umbra/umath.h
		include/umbra/umbragl.h
		include/umbra/umbra.h

		src/uwindow.c
		src/renderer.c
		src/umath.c
)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw glad)
# target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/static/freetype.lib)

target_include_directories(
	${PROJECT_NAME}
	
	PUBLIC include/umbra
	private lib/freetype
	private lib/glad/include
	private lib/stb
)
