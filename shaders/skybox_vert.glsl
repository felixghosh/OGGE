#version 450 core
layout (location = 0) in vec4 vPos;

out vec3 TexCoords;

uniform mat4 mvp;

void main()
{
    TexCoords = vPos.xyz;
    gl_Position = mvp*vPos;
}  