#include <stdio.h>
#include <stdlib.h>

#include "uinput.h"
#include "umbra.h"

bool U_InitWindow(const char* title, int width, int height)
{
	if(!glfwInit())
	{
		fprintf(stderr, "An error occurred while initializing window");
		return NULL;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* glfw_handler = glfwCreateWindow(
			width,
			height,
			title,
			NULL, NULL);

	if(glfw_handler == NULL)
	{
		fprintf(stderr, "Could not initialize window");
		return NULL;
	}

	// Todo: Check for OpenGL Errors

	UCORE.window._glfw_handler = glfw_handler;

	glfwMakeContextCurrent(glfw_handler);

	UCORE.window.title = title;
	UCORE.window.width = width;
	UCORE.window.height = height;

	UCORE.window.exit = false;

	gladLoadGL();

	// Init default renderering context
	U_InitRenderer(&UCORE.renderer, true /* TODO: Load value from flag */);

	// Set window clip space
	U_SetViewport(&UCORE.renderer, 0, width, height, 0, 1, -1);

	// Ensure key states are initialized to false
	for(int i = 0; i < U_NUM_SUPPORTED_KEYS; i++)
	{
		UCORE.input.current_frame_keys[i] = false;
	}

	// Set input callback
	glfwSetKeyCallback(glfw_handler, &U_KeyCallback);

	return true;
}

bool U_ShouldCloseWindow()
{
	glfwSwapBuffers(UCORE.window._glfw_handler);

	U_PollWindowEvents();
	return glfwWindowShouldClose(UCORE.window._glfw_handler) || UCORE.window.exit;
}

void U_PollWindowEvents()
{
	glfwPollEvents();
}

void U_CloseWindow()
{
	glfwDestroyWindow(UCORE.window._glfw_handler);
	glfwTerminate();
	// free(window_ptr);
}