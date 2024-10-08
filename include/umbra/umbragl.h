#pragma once

/** Wrapper for OpenGl */
#include "export_macros.h"
#include <stdbool.h>
#include "gl_include.h"
#include "umath.h"
#include "types.h"

#ifndef U_MAX_VERTEX
    #define U_MAX_VERTEX 2048
#endif
#ifndef U_MAX_BUFFERS
    #define U_MAX_BUFFERS 255
#endif

#define U_MAX_VERTEX_ATTRIBUTES GL_MAX_VERTEX_ATTRIBS - 1

/* OpengGL Redefines */
#define U_FRAGMENT_SHADER 0x8B30 // Redefine for 'GL_FRAGMENT_SHADER'
#define U_VERTEX_SHADER 0x8B31 // Redefine for 'GL_VERTEX_SHADER'

/* Types and Structures definitions */
#define U_DEFAULT_VERTEX_ATTR_POSITION_IDX 0
#define U_DEFAULT_VERTEX_ATTR_COLOR_IDX 1

typedef ui32 TextureID;

typedef struct UVertexAttribute
{
    ui64 index;
    i64 size;
    ui64 type;
    bool normalized;
    ui64 stride;
    ui32 offset; // also known as pointer
} UVertexAttribute;

typedef struct UVertexArrayObject
{
    ui8 id;
    ui64 enabled_attributes[U_MAX_VERTEX_ATTRIBUTES];

    ui64 _attrs_counter;
    ui64 _enabled_attrs_counter;
    UVertexAttribute _attributes[U_MAX_VERTEX_ATTRIBUTES];

} UVertextArrayObject;


typedef struct UWindow
{
    GLFWwindow* _glfw_handler;
    const char* title;
    int width;
    int height;
    bool exit;
} UWindow;

typedef struct GLState
{
    ui8 current_vao;
    ui8 current_vbo;

    ui8 _vaos_counter;
    UVertextArrayObject vaos[U_MAX_BUFFERS];
    ui8 vbos[U_MAX_BUFFERS];
} GLState;

extern GLState G_GL_STATE;

#ifdef __cplusplus
extern "C" {
#endif

/* OpenGL Wrapper functions */
UMBRA_API GLFWwindow* U_GetGLFWHandler();
UMBRA_API void U_PollWindowEvents();

UMBRA_API void U_SetVertexAttribute(ui64 idx, i64 size, ui64 type, bool normalized, ui64 stride, ui64 offset);
UMBRA_API void U_EnableVertexAttribute(ui64 idx);

UMBRA_API void U_GenerateVAO(ui32* buffer);
UMBRA_API void U_BindVertexArray(ui32 vao);
/**
    A call to glBindBuffer to set the GL_ARRAY_BUFFER binding is NOT modifying the current VAO's state!
    https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Array_Object
*/
UMBRA_API void U_BindVBO(ui32 vbo);
UMBRA_API void U_BindEBO(ui32 ebo);
UMBRA_API void U_BindTexture2D(TextureID id);
// State setting
UMBRA_API void U_EnableBlending();
UMBRA_API void U_DisableBlending();
UMBRA_API void U_EnableDepthTest();
UMBRA_API void U_DisableDepthTest();

// Rendering
UMBRA_API void U_SwapBuffers(UWindow window);
UMBRA_API void U_ClearColor(f32 red, f32 green, f32 blue, f32 alpha);
UMBRA_API void U_SetViewport(f32 left, f32 right, f32 bottom, f32 top, f32 clip_near, f32 clip_far);
UMBRA_API void U_SetDepthRangeF(f32 near, f32 far);

#ifdef __cplusplus
}
#endif
