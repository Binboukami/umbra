#include "umath.h"

UMat4x4 U_Mat4x4()
{
    UMat4x4 matrix;

    for (int i = 0;  i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            f32 v = 0.0f;
            if(i == j) v = 1.0f;

            matrix.data[i][j] = v;
        }
    }

    return matrix;
}

UMat4x4 U_Translate(UMat4x4 mat, UVec3 vec3)
{
    // Copy mat
    UMat4x4 translated = mat;

    translated.data[0][3] = vec3.x;
    translated.data[1][3] = vec3.y;
    translated.data[2][3] = vec3.z;

    return translated;
}