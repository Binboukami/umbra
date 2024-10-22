#version 430 core

in vec3 vColor;
in vec2 vTextCoord;
in float vTextIdx;

out vec4 FragColor;

uniform sampler2D u_TextureArray[32];

void main()
{
    int idx = int(vTextIdx);

    if(idx < 0)
    {
        FragColor = vec4(vColor, 1.0f);
    }
    else
    {
        FragColor = texture(u_TextureArray[idx], vTextCoord);
    }
}
