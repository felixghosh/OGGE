#version 450 core

in vec4 color;
in vec3 normal;
in vec3 world_pos;

out vec4 FragColor;

void main()
{
    FragColor = vec4(color.xyz, 1.0);
}