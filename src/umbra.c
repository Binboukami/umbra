#include "umbra.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static UCoreContext *u_lib_instance_ = NULL;

U_InitContextResult U_InitContext() {
	U_InitContextResult result;

	/** Initialize glfw subsystem **/
	if (!glfwInit()) {
		fprintf(stderr, "An error occurred while initializing window");
		result.error_ = -1; // TODO: Add define for GLFW error subsys
		return result;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	u_lib_instance_ = malloc(sizeof(UCoreContext));

	if (u_lib_instance_ == NULL) // Ran out of RAM
		result.error_ = -2; // TODO: Add define for error

	memset(u_lib_instance_, 0, sizeof(UCoreContext));

	result.uc_ = u_lib_instance_;

	return result;
};

UCoreContextRef U_GetInstance() {
	// TODO: Standardize error handling
	assert(u_lib_instance_ != NULL);
	return u_lib_instance_;
};

void U_DestroyContext() {
	if (u_lib_instance_ != NULL)
		free(u_lib_instance_);
}
