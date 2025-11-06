#version 450 core
layout (location = 0) in vec4 vPos;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec3 vNormal;

out vec4 color;

layout(std140, binding = 0) uniform vert_data {
    mat4 model_mat;    //Unused
    mat4 mvp;
    int is_textured_u; //Unused
};

void main()
{
    gl_Position = mvp*vPos;

    color = vColor;
}