#version 430 core

layout (location = 0) in vec3 absPos;
layout (location = 1) in vec3 absColor;

uniform mat4 UNIFORM_MATRIX_PROJECTION;
uniform mat4 UNIFORM_MATRIX_VIEW;
uniform mat4 UNIFORM_MATRIX_MODEL;

out vec3 Color;

void main()
{
  gl_Position = vec4(absPos, 1.0f) * UNIFORM_MATRIX_MODEL;
}
