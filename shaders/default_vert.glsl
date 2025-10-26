#version 450 core
layout (location = 0) in vec4 vPos;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec2 vTexCoord;

out vec4 color;
out vec3 normal;
out vec3 world_pos;
out vec2 tex_coord;

uniform mat4 model_mat;
uniform mat4 mvp;


void main()
{
    gl_Position = mvp*vPos;

    color = vColor;
    normal = vNormal;
    world_pos = (model_mat * vPos).xyz;
    tex_coord = vTexCoord;
}