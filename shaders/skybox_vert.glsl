#version 450 core
layout (location = 0) in vec4 vPos;

out vec3 TexCoords;

layout(std140, binding = 0) uniform vert_data {
    mat4 model_mat;    //Unused
    mat4 mvp;
    int is_textured_u; //Unused
};

void main()
{
    TexCoords = vPos.xyz;
    gl_Position = mvp*vPos;
}  