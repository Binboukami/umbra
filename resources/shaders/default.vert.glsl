#version 430 core

layout (location = 0) in vec3 absPos;
layout (location = 1) in vec3 absColor;
layout (location = 2) in vec2 absTextCoord;
layout (location = 3) in float absTextIndex;

uniform mat4 UNIFORM_MATRIX_PROJECTION;
uniform mat4 UNIFORM_MATRIX_VIEW;
uniform mat4 UNIFORM_MATRIX_MODEL;

out vec3 vColor;
out vec2 vTextCoord;
out float vTextIdx;

void main()
{
  gl_Position = UNIFORM_MATRIX_PROJECTION * UNIFORM_MATRIX_VIEW * UNIFORM_MATRIX_MODEL * vec4(absPos, 1.0f);


  vTextIdx = absTextIndex;
	vColor = absColor;
  vTextCoord = absTextCoord;
}
