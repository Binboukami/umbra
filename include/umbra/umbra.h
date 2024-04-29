#pragma once

/* Macro definitions */
#include "export_macros.h"
#include "umbragl.h"

/* Structure defitions */

typedef struct UWindow
{
    GLFWwindow* _glfw_handler;
    const char* title;
    int width;
    int height;
    bool exit;
} UWindow;

#ifdef __cplusplus
extern "C" {
#endif

/* Function definitions */

/* Window API */

UMBRA_API UWindow* U_InitWindow(const char* title, int width, int height);
UMBRA_API GLFWwindow* U_GetGLFWHandler();
UMBRA_API bool U_ShouldCloseWindow(const UWindow* window_ptr);
UMBRA_API void U_PollWindowEvents();
UMBRA_API void U_CloseWindow(UWindow* window_ptr);

#ifdef __cplusplus
}
#endif
