#pragma once

#include "export_macros.h"
#include "types.h"
#include "gl_include.h"
#include <stdbool.h>

#define U_NUM_SUPPORTED_KEYS 120

#ifndef U_MAX_KEYS_PRESSED
#define U_MAX_KEYS_PRESSED 4
#endif

/** Renamed GLFW defined keys */
#define U_KEY_SPACE              KEY_SPACE
#define U_KEY_APOSTROPHE         GLFW_KEY_APOSTROPHE  /* ' */
#define U_KEY_COMMA              GLFW_KEY_COMMA  /* , */
#define U_KEY_MINUS              GLFW_KEY_MINUS  /* - */
#define U_KEY_PERIOD             GLFW_KEY_PERIOD  /* . */
#define U_KEY_SLASH              GLFW_KEY_SLASH  /* / */
#define U_KEY_0                  GLFW_KEY_0
#define U_KEY_1                  GLFW_KEY_1
#define U_KEY_2                  GLFW_KEY_2
#define U_KEY_3                  GLFW_KEY_3
#define U_KEY_4                  GLFW_KEY_4
#define U_KEY_5                  GLFW_KEY_5
#define U_KEY_6                  GLFW_KEY_6
#define U_KEY_7                  GLFW_KEY_7
#define U_KEY_8                  GLFW_KEY_8
#define U_KEY_9                  GLFW_KEY_9
#define U_KEY_SEMICOLON          GLFW_KEY_SEMICOLON  /* ; */
#define U_KEY_EQUAL              GLFW_KEY_EQUAL  /* = */
#define U_KEY_A                  GLFW_KEY_A
#define U_KEY_B                  GLFW_KEY_B
#define U_KEY_C                  GLFW_KEY_C
#define U_KEY_D                  GLFW_KEY_D
#define U_KEY_E                  GLFW_KEY_E
#define U_KEY_F                  GLFW_KEY_F
#define U_KEY_G                  GLFW_KEY_G
#define U_KEY_H                  GLFW_KEY_H
#define U_KEY_I                  GLFW_KEY_I
#define U_KEY_J                  GLFW_KEY_J
#define U_KEY_K                  GLFW_KEY_K
#define U_KEY_L                  GLFW_KEY_L
#define U_KEY_M                  GLFW_KEY_M
#define U_KEY_N                  GLFW_KEY_N
#define U_KEY_O                  GLFW_KEY_O
#define U_KEY_P                  GLFW_KEY_P
#define U_KEY_Q                  GLFW_KEY_Q
#define U_KEY_R                  GLFW_KEY_R
#define U_KEY_S                  GLFW_KEY_S
#define U_KEY_T                  GLFW_KEY_T
#define U_KEY_U                  GLFW_KEY_U
#define U_KEY_V                  GLFW_KEY_V
#define U_KEY_W                  GLFW_KEY_W
#define U_KEY_X                  GLFW_KEY_X
#define U_KEY_Y                  GLFW_KEY_Y
#define U_KEY_Z                  GLFW_KEY_Z
#define U_KEY_LEFT_BRACKET       GLFW_KEY_LEFT_BRACKET  /* [ */
#define U_KEY_BACKSLASH          GLFW_KEY_BACKSLASH  /* \ */
#define U_KEY_RIGHT_BRACKET      GLFW_KEY_RIGHT_BRACKET  /* ] */
#define U_KEY_GRAVE_ACCENT       GLFW_KEY_GRAVE_ACCENT  /* ` */
#define U_KEY_WORLD_1            GLFW_KEY_WORLD_1 /* non-US #1 */
#define U_KEY_WORLD_2            GLFW_KEY_WORLD_2 /* non-US #2 */

/* Function keys */
#define U_KEY_ESCAPE             GLFW_KEY_ESCAPE
#define U_KEY_ENTER              GLFW_KEY_ENTER
#define U_KEY_TAB                GLFW_KEY_TAB
#define U_KEY_BACKSPACE          GLFW_KEY_BACKSPACE
#define U_KEY_INSERT             GLFW_KEY_INSERT
#define U_KEY_DELETE             GLFW_KEY_DELETE
#define U_KEY_RIGHT              GLFW_KEY_RIGHT
#define U_KEY_LEFT               GLFW_KEY_LEFT
#define U_KEY_DOWN               GLFW_KEY_DOWN
#define U_KEY_UP                 GLFW_KEY_UP
#define U_KEY_PAGE_UP            GLFW_KEY_PAGE_UP
#define U_KEY_PAGE_DOWN          GLFW_KEY_PAGE_DOWN
#define U_KEY_HOME               GLFW_KEY_HOME
#define U_KEY_END                GLFW_KEY_END
#define U_KEY_CAPS_LOCK          GLFW_KEY_CAPS_LOCK
#define U_KEY_SCROLL_LOCK        GLFW_KEY_SCROLL_LOCK
#define U_KEY_NUM_LOCK           GLFW_KEY_NUM_LOCK
#define U_KEY_PRINT_SCREEN       GLFW_KEY_PRINT_SCREEN
#define U_KEY_PAUSE              GLFW_KEY_PAUSE
#define U_KEY_F1                 GLFW_KEY_F1
#define U_KEY_F2                 GLFW_KEY_F2
#define U_KEY_F3                 GLFW_KEY_F3
#define U_KEY_F4                 GLFW_KEY_F4
#define U_KEY_F5                 GLFW_KEY_F5
#define U_KEY_F6                 GLFW_KEY_F6
#define U_KEY_F7                 GLFW_KEY_F7
#define U_KEY_F8                 GLFW_KEY_F8
#define U_KEY_F9                 GLFW_KEY_F9
#define U_KEY_F10                GLFW_KEY_F10
#define U_KEY_F11                GLFW_KEY_F11
#define U_KEY_F12                GLFW_KEY_F12
#define U_KEY_F13                GLFW_KEY_F13
#define U_KEY_F14                GLFW_KEY_F14
#define U_KEY_F15                GLFW_KEY_F15
#define U_KEY_F16                GLFW_KEY_F16
#define U_KEY_F17                GLFW_KEY_F17
#define U_KEY_F18                GLFW_KEY_F18
#define U_KEY_F19                GLFW_KEY_F19
#define U_KEY_F20                GLFW_KEY_F20
#define U_KEY_F21                GLFW_KEY_F21
#define U_KEY_F22                GLFW_KEY_F22
#define U_KEY_F23                GLFW_KEY_F23
#define U_KEY_F24                GLFW_KEY_F24
#define U_KEY_F25                GLFW_KEY_F25
#define U_KEY_KP_0               GLFW_KEY_KP_0
#define U_KEY_KP_1               GLFW_KEY_KP_1
#define U_KEY_KP_2               GLFW_KEY_KP_2
#define U_KEY_KP_3               GLFW_KEY_KP_3
#define U_KEY_KP_4               GLFW_KEY_KP_4
#define U_KEY_KP_5               GLFW_KEY_KP_5
#define U_KEY_KP_6               GLFW_KEY_KP_6
#define U_KEY_KP_7               GLFW_KEY_KP_7
#define U_KEY_KP_8               GLFW_KEY_KP_8
#define U_KEY_KP_9               GLFW_KEY_KP_9
#define U_KEY_KP_DECIMAL         GLFW_KEY_KP_DECIMAL
#define U_KEY_KP_DIVIDE          GLFW_KEY_KP_DIVIDE
#define U_KEY_KP_MULTIPLY        GLFW_KEY_KP_MULTIPLY
#define U_KEY_KP_SUBTRACT        GLFW_KEY_KP_SUBTRACT
#define U_KEY_KP_ADD             GLFW_KEY_KP_ADD
#define U_KEY_KP_ENTER           GLFW_KEY_KP_ENTER
#define U_KEY_KP_EQUAL           GLFW_KEY_KP_EQUAL
#define U_KEY_LEFT_SHIFT         GLFW_KEY_LEFT_SHIFT
#define U_KEY_LEFT_CONTROL       GLFW_KEY_LEFT_CONTROL
#define U_KEY_LEFT_ALT           GLFW_KEY_LEFT_ALT
#define U_KEY_LEFT_SUPER         GLFW_KEY_LEFT_SUPER
#define U_KEY_RIGHT_SHIFT        GLFW_KEY_RIGHT_SHIFT
#define U_KEY_RIGHT_CONTROL      GLFW_KEY_RIGHT_CONTROL
#define U_KEY_RIGHT_ALT          GLFW_KEY_RIGHT_ALT
#define U_KEY_RIGHT_SUPER        GLFW_KEY_RIGHT_SUPER
#define U_KEY_MENU               GLFW_KEY_MENU

typedef struct UInputHandler {
	ui32 current_frame_keys[U_NUM_SUPPORTED_KEYS];
	bool is_modifier_pressed;
	ui32 current_frame_modifier[3]; // CTRL | SHIFT | ALT
} UInputHandler;

#ifdef __cplusplus
extern "C" {
#endif

ui32 U_IsKeyDown(ui32 key);

/** Internals */
void U_KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

#ifdef __cplusplus
}
#endif
