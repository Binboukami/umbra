#version 430 core

out vec4 FragColor;

uniform vec4 uMeshColor;

void main()
{
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f) * uMeshColor;
}