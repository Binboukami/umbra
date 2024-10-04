#include "umath.h"
#include <stdlib.h>
#include <string.h>

UMat4x4 U_Mat4x4(const f32 scalar)
{
    UMat4x4 matrix;

	memset(matrix.data, 0.0f, sizeof(matrix.data));

	matrix.data[0][0] = scalar;
	matrix.data[1][1] = scalar;
	matrix.data[2][2] = scalar;
	matrix.data[3][3] = scalar;

	return matrix;
}

UMat4x4 U_Translate(const UMat4x4 mat, UVec3 vec3)
{
    UMat4x4 translated = mat; // Copy matrix

    translated.data[0][3] = vec3.x;
    translated.data[1][3] = vec3.y;
    translated.data[2][3] = vec3.z;

    translated.data[3][3] = 1;

    return translated;
}

UMat4x4 U_Scale(const UMat4x4 mat, const UVec3 vec3)
{
    UMat4x4 scaled = mat; // Copy matrix

    scaled.data[0][0] = vec3.x;
    scaled.data[1][1] = vec3.y;
    scaled.data[2][2] = vec3.z;

    scaled.data[3][3] = 1;

    return scaled;
}

// TODO
UMat4x4 U_MatMult(const UMat4x4 matA, const UMat4x4 matB)
{
	UMat4x4 mat;

	for(int i = 0; i < 4; i ++)
	{
		for (int j = 0; j < 4; j++)
		{
			mat.data[i][j] += matA.data[i][j] * matB.data[j][i];
		}
	}

	return mat;
}

UMat4x4 U_MatOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far)
{
	UMat4x4 ortho = U_Mat4x4(1.0f);

	ortho.data[0][0] = 2 / (right - left);
	ortho.data[1][1] = 2 / (top - bottom);
	ortho.data[2][2] = -2 / (far - near);

	ortho.data[0][3] = -((right + left) / (right - left));
	ortho.data[1][3] = -((top + bottom) / (top - bottom));
	ortho.data[2][3] = - (far + near ) / 2;

	return ortho;
}

UMat4x4 U_MatPerspective(f32 aspect_ratio, f32 fov, f32 near, f32 far)
{
	UMat4x4 p = U_Mat4x4(1.0f);

	f32 f = 1 / tan(fov/2);

	p.data[0][0] = aspect_ratio * f ;
	p.data[1][1] = f;
	p.data[2][2] = far / (far-near);
	p.data[2][3] = (-far * near) / (far-near);

	p.data[3][2] = 1;

	return p;
}
