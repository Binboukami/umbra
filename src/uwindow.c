#include <stdio.h>
#include <stdlib.h>
#include "uinput.h"
#include "umbra.h"

bool U_InitWindow(const char *title, const int width, const int height) {
	UCoreContext *context = U_GetInstance();

	GLFWwindow *glfw_handler = glfwCreateWindow(
		width,
		height,
		title,
		NULL, NULL);

	if (glfw_handler == NULL) {
		fprintf(stderr, "Could not initialize window");
		return NULL;
	}

	// Todo: Check for OpenGL Errors
	glfwMakeContextCurrent(glfw_handler);

	context->window = (UWindow){
		.title = title,
		.width = width,
		.height = height,

		._glfw_handler =  glfw_handler,
		.exit = false
	};

	gladLoadGL();

	// Init default rendering context
	U_InitRenderer(&context->renderer, true /* TODO: Load value from flag */);

	// Set window clip space
	U_SetViewport3D(0, 0, width, height, 1, -1);

	// Ensure key states are initialized to false
	for (int i = 0; i < U_NUM_SUPPORTED_KEYS; i++) {
		context->input.current_frame_keys[i] = false;
	}

	// Set input callback
	glfwSetKeyCallback(glfw_handler, &U_KeyCallback);

	return true;
}

bool U_ShouldCloseWindow() {
	const UCoreContextRef context = U_GetInstance();

	U_SwapBuffers(context->window);

	U_PollWindowEvents();
	return glfwWindowShouldClose(context->window._glfw_handler) || context->window.exit;
}

void U_CloseWindow() {
	const UCoreContextRef context = U_GetInstance();

	glfwDestroyWindow(context->window._glfw_handler);
	glfwTerminate();
}
