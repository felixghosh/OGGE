#version 450 core
layout (location = 0) in vec4 vPos;
layout (location = 1) in vec4 vColor;

out vec4 ourColor;

uniform mat4 ctm;


void main()
{
    vec3 temp = vPos.xyz * 0.4;
    gl_Position = ctm*vec4(temp, 1.0);
    ourColor = vColor;
}