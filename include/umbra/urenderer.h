#pragma once

#include <stdbool.h>
#include "export_macros.h"
#include "types.h"
#include "umath.h"

#ifndef U_MAX_VERTEX
    #define U_MAX_VERTEX 2048
#endif

typedef ui32 U_SHADER_TYPE;

typedef struct UVertex
{
    UVec3 position;
    UVec3 color;
} UVertex;

typedef struct URenderer {
    ui8 id;

    ui32 vbo;
    ui32 ebo;
    ui32 vao;

    bool use_ebo;

    ui32 vertex_count;
    ui32 index_count;
    UVertex buffer[U_MAX_VERTEX];
    ui32 index_buffer[U_MAX_VERTEX * 2];

    ui64 shader_id;
} URenderer;

#ifdef __cplusplus
extern "C" {
#endif

UMBRA_API void U_InitRenderer(URenderer* renderer, ui8 use_ebo);
/* Drawing: Primitives */
UMBRA_API void U_DrawTris(URenderer* renderer, const UVec3 position, const f32 size, const UVec3 color);
UMBRA_API void U_DrawQuad(URenderer* renderer, const UVec3 position, const f32 size, const UVec3 color);
UMBRA_API void U_DrawRect(URenderer* renderer, const UVec3 position, const f32 width, const f32 height, const UVec3 color);

UMBRA_API void U_DrawCube(URenderer* renderer, const UVec3 position, const f32 size, const UVec3 color);

/* Shader */
UMBRA_API i32 U_LoadShader(const char* filepath, U_SHADER_TYPE shader_type);
UMBRA_API ui32 U_CompileShaderProgram(i32 vertex_shader, i32 fragment_shader); // todo: This function consumes the shader src, making them unusable after call
UMBRA_API void U_UseShader(URenderer* renderer, ui32 shader_id);

#ifdef __cplusplus
}
#endif