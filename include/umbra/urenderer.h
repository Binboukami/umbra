#pragma once

#include <stdbool.h>
#include "export_macros.h"
#include "types.h"
#include "umath.h"
#include "umbragl.h"

#ifndef U_MAX_VERTEX
    #define U_MAX_VERTEX 2048
#endif

enum UProjectionType {
	U_ORTHO = 0,
	U_PERSPECTIVE = 1
};

typedef struct UCamera {
	UVec3 position;
	enum UProjectionType projection;
	f32 fov;
} UCamera;

typedef struct UVertex {
	UVec3 position;
	UVec3 color;
	UVec2 text_coord;
	TextureID texture_id;
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

	ui32 samples_count;
	int samples[32];

	ui64 shader_id;

	TextureID teste_texture;
} URenderer;

#ifdef __cplusplus
extern "C" {
#endif

UMBRA_API void U_InitRenderer(URenderer *renderer, ui8 use_ebo);

/* General */
UMBRA_API void U_BeginDrawing(UCamera camera);

UMBRA_API void U_EndDrawing();

UMBRA_API TextureID LoadTexture(const char *path);

/* Drawing: Primitives */
UMBRA_API void U_DrawTris(URenderer *renderer, const UVec3 position, const f32 size, const UVec3 color);

UMBRA_API void U_DrawQuad(URenderer *renderer, const UVec3 position, const f32 size, const UVec3 color);

UMBRA_API void U_DrawTextureQuad(URenderer *renderer, const UVec3 position, const f32 size, TextureID texture);

UMBRA_API void U_DrawRect(URenderer *renderer, const UVec3 position, const f32 width, const f32 height,
                          const UVec3 color);

UMBRA_API void U_DrawCube(URenderer *renderer, const UVec3 position, const f32 size, const UVec3 color);

/* Shader */
UMBRA_API void U_UseShader(URenderer *renderer, ui32 shader_id);

#ifdef __cplusplus
}
#endif
