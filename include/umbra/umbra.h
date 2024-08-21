#pragma once

/* Macro definitions */
#include "export_macros.h"
#include "umath.h"
#include "umbragl.h"

/* Structure defitions */
#define MAX_SUPPORTED_RENDERERS 8

typedef struct UWindow
{
    GLFWwindow* _glfw_handler;
    const char* title;
    int width;
    int height;
    bool exit;
} UWindow;

typedef struct UCoreContext
{
    UWindow window;
    URenderer renderer;

} UCoreContext;

extern UCoreContext UCORE; // Global library context

#ifdef __cplusplus
extern "C" {
#endif

/* Function definitions */

/* Core API */
// TODO

/* Window API */

UMBRA_API bool U_InitWindow(const char* title, int width, int height);
UMBRA_API GLFWwindow* U_GetGLFWHandler();
UMBRA_API bool U_ShouldCloseWindow();
UMBRA_API void U_PollWindowEvents();
UMBRA_API void U_CloseWindow();

/* Umbra Math */
UMBRA_API UMat4x4 U_Mat4x4(const f32 scalar); // redefine export
UMBRA_API UMat4x4 U_Translate(const UMat4x4 mat, const UVec3 vec3); // redefine export
UMBRA_API UMat4x4 U_Scale(const UMat4x4 mat, const UVec3 vec3);
UMBRA_API UMat4x4 U_MatMult(const UMat4x4 matA, const UMat4x4 matB);
UMBRA_API UMat4x4 U_MatOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);

#ifdef __cplusplus
}
#endif
