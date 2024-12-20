#pragma once

/* Macro definitions */
#include "export_macros.h"
#include "umath.h"
#include "uinput.h"
#include "urenderer.h"
#include "umbragl.h"

/* Structure definitions */
#define MAX_SUPPORTED_RENDERERS 8
#define ORTHO_CLIPPING_SPACE 10.0f

typedef struct UCoreContext {
	UWindow window;
	URenderer renderer;
	UInputHandler input;
} UCoreContext;

typedef UCoreContext *UCoreContextRef;

typedef union {
	UCoreContext *uc_;
	ui8 error_;
} U_InitContextResult;

#ifdef __cplusplus
extern "C" {
#endif

/* Context handling */
UMBRA_API U_InitContextResult U_InitContext();

UMBRA_API UCoreContextRef U_GetInstance();
UMBRA_API void U_DestroyContext();

/* Window API */
UMBRA_API bool U_InitWindow(const char *title, int width, int height);

UMBRA_API bool U_ShouldCloseWindow();

UMBRA_API void U_CloseWindow();

/* Umbra Math redefines for bundling */
UMBRA_API UMat4x4 U_Mat4x4(const f32 scalar); // redefine export
UMBRA_API UMat4x4 U_Translate(const UMat4x4 mat, const UVec3 vec3); // redefine export
UMBRA_API UMat4x4 U_Scale(const UMat4x4 mat, const UVec3 vec3);

UMBRA_API UMat4x4 U_MatMult(const UMat4x4 matA, const UMat4x4 matB);

UMBRA_API UMat4x4 U_MatOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);

/** Input */
UMBRA_API ui32 U_IsKeyDown(ui32 key);

#ifdef __cplusplus
}
#endif
