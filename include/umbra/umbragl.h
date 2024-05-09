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

/* OpengGL Redefines */
#define U_FRAGMENT_SHADER 0x8B30 // Redefine for 'GL_FRAGMENT_SHADER'
#define U_VERTEX_SHADER 0x8B31 // Redefine for 'GL_VERTEX_SHADER'

/* Types and Structures definitions */
typedef ui32 U_SHADER_TYPE;

typedef struct Transform
{
    UVec2 position;
    UVec2 size;
} Transform;

typedef ui32 TextureID;

typedef struct Texture
{
    TextureID id;
} Texture;

typedef struct UVertex
{
    UVec3 position;
    UVec3 color;
} UVertex;

typedef struct URenderer {
    ui32 vbo;
    ui32 ebo;
    ui32 vao;

    ui32 vertex_count;
    UVertex buffer[U_MAX_VERTEX];

    ui64 shader_id;
} URenderer;

#ifdef __cplusplus
extern "C" {
#endif

/* Function definitions */
/* General */
UMBRA_API void U_InitRenderer(URenderer* renderer);
UMBRA_API void U_BeginDrawing(URenderer* renderer);
UMBRA_API void U_EndDrawing(URenderer* renderer);
UMBRA_API void U_ClearColor(f32 red, f32 green, f32 blue, f32 alpha);
UMBRA_API void U_SetViewport(f32 left, f32 right, f32 bottom, f32 top, f32 clip_near, f32 clip_far);

/* Drawing: Primitives */
UMBRA_API void U_DrawTris(URenderer* renderer, UVec3 position, UVec3 color);

/* Shader */
UMBRA_API i32 U_LoadShader(const char* filepath, U_SHADER_TYPE shader_type);
UMBRA_API void U_UseShader(URenderer* renderer, ui32 shader_id);
UMBRA_API ui32 U_CompileShaderProgram(i32 vertex_shader, i32 fragment_shader); // todo: This function consumes the shader src, making them unusable after call

/* OpenGL Wrapper functions */
UMBRA_API void U_BindVertexArray(ui32 vao);
UMBRA_API void U_BindTexture2D(TextureID id);
UMBRA_API void U_EnableBlending();
UMBRA_API void U_DisableBlending();

#ifdef __cplusplus
}
#endif
