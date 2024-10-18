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

/* OpenGL Redefines */
#define U_FRAGMENT_SHADER 0x8B30 // Redefine for 'GL_FRAGMENT_SHADER'
#define U_VERTEX_SHADER 0x8B31 // Redefine for 'GL_VERTEX_SHADER'

/* Types and Structures definitions */
#define U_DEFAULT_VERTEX_ATTR_POSITION_IDX 0
#define U_DEFAULT_VERTEX_ATTR_COLOR_IDX 1

typedef ui32 TextureID;
typedef ui32 U_SHADER_TYPE;

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

} UVertexArrayObject;


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
    UVertexArrayObject vaos[U_MAX_BUFFERS];
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

// Shading
UMBRA_API i32 U_LoadShader(const char* filepath, U_SHADER_TYPE shader_type);
UMBRA_API ui32 U_CompileShaderProgram(i32 vertex_shader, i32 fragment_shader); // todo: This function consumes the shader src, making them unusable after call

// State setting
UMBRA_API void U_EnableBlending();
UMBRA_API void U_DisableBlending();
UMBRA_API void U_EnableDepthTest();
UMBRA_API void U_DisableDepthTest();
UMBRA_API void U_EnableScissor();
UMBRA_API void U_DisableScissor();

// Rendering
UMBRA_API void U_SwapBuffers(UWindow window);
UMBRA_API void U_ClearColor(f32 red, f32 green, f32 blue, f32 alpha);
UMBRA_API void U_SetViewport(const f32 left, const f32 right, const f32 bottom, const f32 top);
UMBRA_API void U_SetViewport3D(f32 left, f32 right, f32 bottom, f32 top, f32 clip_near, f32 clip_far);
UMBRA_API void U_SetDepthRangeF(f32 near, f32 far);

/** GL_SCISSOR_TEST has to be enabled */
UMBRA_API void U_Scissor(f32 x, f32 y, f32 width, f32 height);

#ifdef __cplusplus
}
#endif
