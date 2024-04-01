#version 450 core
out vec4 FragColor;  

in vec3 ourColor;

uniform float b_comp;
  
void main()
{
    FragColor = vec4(ourColor.xy, b_comp, 1.0);
}