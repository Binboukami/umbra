#pragma once

#include "export_macros.h"
#include "types.h"

/* Structure defitions */

typedef struct UVec2
{
    f32 x; f32 y;
} UVec2;

typedef struct UVec3
{
    f32 x; f32 y; f32 z;
} UVec3;

typedef struct UMat4x4
{
    f32 data[4][4];
} UMat4x4;

#ifdef __cplusplus
extern "C" {
#endif

/* Function definitions */
/* Matrix manipulation */

UMat4x4 U_Mat4x4();
UMat4x4 U_Translate(UMat4x4 mat, UVec3 vec3);

#ifdef __cplusplus
}
#endif
