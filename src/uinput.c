#include "uinput.h"
#include "GLFW/glfw3.h"
#include "umbra.h"

// TODO: Takes in wrapped GLFW_KEYs
ui32 U_IsKeyDown(ui32 key) {
	const UCoreContextRef u_instance = U_GetInstance();
	return u_instance->input.current_frame_keys[key];
}

void U_KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	const UCoreContextRef u_instance = U_GetInstance();
	if (action == GLFW_PRESS) {
		u_instance->input.current_frame_keys[key] = true;
	}

	if (action == GLFW_RELEASE) {
		u_instance->input.current_frame_keys[key] = false;
	}
}
