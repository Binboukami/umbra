#version 430 core

out vec4 FragColor;

in vec3 Color;
in vec2 TexCoord;

uniform vec4 meshColor;
uniform sampler2D meshTexture;

void main()
{
  FragColor = texture(meshTexture, TexCoord) * (vec4(Color, 1.0f) * meshColor);
}