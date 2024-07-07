#version 450 core
layout (location = 0) in vec4 vPos;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec3 vNormal;

out vec4 color;
out vec3 normal;
out vec3 world_pos;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;


void main()
{
    mat4 mvp = projection_mat * view_mat * model_mat;
    vec3 temp = vPos.xyz * 0.4;
    gl_Position = mvp*vec4(temp, 1.0);

    color = vColor;
    normal = vNormal;
    world_pos = (model_mat * vPos).xyz;
}