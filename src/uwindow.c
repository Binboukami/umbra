#include <stdio.h>
#include <stdlib.h>

#include "umbra.h"

UWindow* U_InitWindow(const char* title, int width, int height)
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

	UWindow* u_window = (UWindow*)malloc(sizeof(UWindow));

	if(u_window == NULL)
	{
		fprintf(stderr, "Failed to initialize UWindow. Please download more ram.");
		return NULL;
	}

	u_window->_glfw_handler = glfw_handler;
	glfwMakeContextCurrent(glfw_handler);

	u_window->title = title;
	u_window->width = width;
	u_window->height = height;

	u_window->exit = false;

	gladLoadGL();

	return u_window;
}

bool U_ShouldCloseWindow(const UWindow* window_ptr)
{
	glfwSwapBuffers(window_ptr->_glfw_handler);

	U_PollWindowEvents();
	return glfwWindowShouldClose(window_ptr->_glfw_handler) || window_ptr->exit;
}

void U_PollWindowEvents()
{
	glfwPollEvents();
}

void U_CloseWindow(UWindow* window_ptr)
{
	glfwDestroyWindow(window_ptr->_glfw_handler);
	glfwTerminate();

	free(window_ptr);
}