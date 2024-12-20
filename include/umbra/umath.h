#pragma once

#include "types.h"

/* Structure defitions */

typedef struct UVec2 {
	f32 x;
	f32 y;
} UVec2;

typedef struct UVec3 {
	f32 x;
	f32 y;
	f32 z;
} UVec3;

typedef struct UMat4x4 {
	f32 data[4][4];
} UMat4x4;

#ifdef __cplusplus
extern "C" {
#endif

/* Function definitions */
/* Matrix manipulation */

UMat4x4 U_Mat4x4(const f32 scalar);

UMat4x4 U_Translate(const UMat4x4 mat, const UVec3 vec3);

UMat4x4 U_Scale(const UMat4x4 mat, const UVec3 vec3);

UMat4x4 U_MatMult(const UMat4x4 matA, const UMat4x4 matB);

/* Projections */
UMat4x4 U_MatOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);

UMat4x4 U_MatPerspective(f32 aspect_ratio, f32 fov, f32 near, f32 far);

// TODO: U_MatIso(...);
UMat4x4 U_PerspectiveDivide(UMat4x4 mat, UVec3 origin);

#ifdef __cplusplus
}
#endif
