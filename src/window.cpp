#include "window.hpp"
#include "gl_include.hpp"

using namespace UECore;

GLFWwindow*
Window::InitWindow()
{
	if(!glfwInit())
	{
		printf("An error occurred while initializing window");
		return NULL;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow* window_handler = glfwCreateWindow(
			this->_width,
			this->_height,
			this->_title,
			NULL, NULL);

	if(!window_handler)
	{
		printf("Could not initialize window");
		return NULL;
	}

	// Todo: Check for OpenGL Errors

	return window_handler;
}

bool
Window::ShouldClose(void)
{
	glfwSwapBuffers(this->_handler);

	this->PollEvents();
	return glfwWindowShouldClose(this->_handler) || this->_exit;
}

void
Window::PollEvents(void)
{
	glfwPollEvents();
}

void
Window::CloseWindow(void)
{
	glfwDestroyWindow(this->_handler);
	glfwTerminate();
}