#pragma once

#include "export_macros.h"
#include "types.h"
#include "gl_include.h"

#ifndef U_MAX_KEYS_PRESSED
    #define U_MAX_KEYS_PRESSED 4
#endif

/** TODO: Key definitions */
#define U_KEY_A 65

typedef struct UInputHandler
{
    ui32 current_frame_keys[67];

} UInputHandler;

#ifdef __cplusplus
extern "C" {
#endif

ui32 U_IsKeyDown(ui32 key);
void U_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods); // Internals

#ifdef __cplusplus
}
#endif
