#include "uinput.h"
#include "umbra.h"
#include <stdio.h>

// TODO: Takes in wrapped GLFW_KEYs
ui32 U_IsKeyDown(ui32 key)
{
    return UCORE.input.current_frame_keys[key];
}

void U_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS)
    {
        UCORE.input.current_frame_keys[key] = true;
    }

    if(action == GLFW_RELEASE)
    {
        UCORE.input.current_frame_keys[key] = false;
    }
}
