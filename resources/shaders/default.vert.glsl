#version 430 core

layout (location = 0) in vec3 absPos;
layout (location = 1) in vec3 absColor;
layout (location = 2) in vec2 absTexCoord;

uniform mat4 UNIFORM_MATRIX_PROJECTION;
uniform mat4 UNIFORM_MATRIX_VIEW;
uniform mat4 UNIFORM_MATRIX_MODEL;

out vec3 Color;
out vec2 TexCoord;

void main()
{
	gl_Position = UNIFORM_MATRIX_PROJECTION * UNIFORM_MATRIX_VIEW * UNIFORM_MATRIX_MODEL * vec4(absPos, 1.0);
	/** Currently used to enable using the same shader for color and textures
			Serves as default multiplier over textures, exhibts the color when multiplied
			over the sample white texture */
	Color = absColor;

	TexCoord = absTexCoord;
}