#version 450 core
layout (location = 0) in vec4 vPos;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec3 vNormal;

out vec4 color;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp*vPos;

    color = vColor;
}