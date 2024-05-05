#version 450 core
layout (location = 0) in vec4 vPos;
layout (location = 1) in vec4 vColor;

out vec4 ourColor;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;


void main()
{
    mat4 mvp = projection_mat * view_mat * model_mat;
    vec3 temp = vPos.xyz * 0.4;
    gl_Position = mvp*vec4(temp, 1.0);
    ourColor = vColor;
}