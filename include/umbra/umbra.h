#pragma once

/* Macro definitions */
#include "export_macros.h"
#include "umath.h"
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

/* Umbra Math */
UMBRA_API UMat4x4 U_Mat4x4(); // redefine export
UMBRA_API UMat4x4 U_Translate(UMat4x4 mat, UVec3 vec3); // redefine export

#ifdef __cplusplus
}
#endif
