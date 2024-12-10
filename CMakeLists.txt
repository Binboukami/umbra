cmake_minimum_required(VERSION 3.1)

include(FetchContent)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For clangd code completion

cmake_policy(SET CMP0063 NEW) # Allows hidding static symbols when exporting library

#region Compiler Flags

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_FLAGS "-Wall -g -Wno-attributes -fPIC")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_FLAGS "-Wall -g -fpermissive -fPIC")

#endregion Compiler Flags

project(
		umbra
		VERSION 0.1.0
		LANGUAGES C CXX
)

#region Build options

set(SHADERS_DIR ${PROJECT_BINARY_DIR}/resources/shaders) # Overridable variable
add_definitions(-DU_SHADERS_DIR="${SHADERS_DIR}") # To enable access in code

#endregion Build options

#region Dependencies

#region GLFW
FetchContent_Declare(
		glfw
		GIT_REPOSITORY https://github.com/glfw/glfw.git
		GIT_TAG 3.3.9 # e2c92645460f680fd272fd2eed591efb2be7dc31
															# OVERRIDE_FIND_PACKAGE -- This line broke on unix
	)

FetchContent_MakeAvailable(glfw)

set(GLFW_BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Hide GLFW symbols
if (GLFW_BUILD_SHARED_LIBS)
	set_property(TARGET glfw PROPERTY C_VISIBILITY_PRESET hidden)
endif()

set(GLFW_INCLUDE_DIR ${CMAKE_BINARY_DIR}/_deps/glfw-src/include)
include_directories(${GLFW_INCLUDE_DIR})

# Comment line below so it builds on unix environment. Review it later
# find_package(GLFW REQUIRED)
#endregion GLFW

#region GLAD
set(GLAD_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/glad)

add_library(
		glad
		STATIC
		${GLAD_LIB_PATH}/src/glad.c)

set_property(TARGET glad PROPERTY C_VISIBILITY_PRESET hidden)
include_directories(${GLAD_LIB_PATH}/include)

#endregion GLAD
#endregion Dependencies

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
	include/umbra/uinput.h
	include/umbra/urenderer.h
	include/umbra/umbra.h
	include/umbra/ugui.h

	src/uwindow.c
	src/urenderer.c
	src/umbragl.c
	src/umath.c
	src/uinput.c
	src/ugui.c
	src/umbra.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

target_include_directories(
	${PROJECT_NAME}

	PUBLIC include/umbra
	PUBLIC lib/freetype
	PUBLIC lib/glad/include
	PUBLIC ${GLFW_INCLUDE_DIR}
	PRIVATE lib/stb
)

# TODO: Copy on changes to resource assets
file(
	COPY
	resources/shaders/default.vert.glsl
	resources/shaders/default.frag.glsl

	DESTINATION ${SHADERS_DIR})

option(BUILD_EXAMPLES "Build example programs" OFF)

if(BUILD_EXAMPLES)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples)
	add_executable(init_window examples/init_window.cpp)

	target_link_libraries(init_window umbra)
endif(BUILD_EXAMPLES)

unset(BUILD_EXAMPLES CACHE)