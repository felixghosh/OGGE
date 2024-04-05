#version 450 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
layout (location = 2) in vec2 aTex; // the tex-coord variable has attribute position 2
  
out vec3 ourColor; // output a color to the fragment shader
out vec2 texCoords;

uniform float time;



void main()
{
    mat3 mx = mat3(
    1.0, 0.0, 0.0, // first column (not row!)
    0.0, cos(time), -0.9*sin(time), // second column
    0.0, 1.7*sin(time), cos(time)  // third column
    );
    mat3 my = mat3(
    cos(time), 0.0, 1.2*sin(time), // first column (not row!)
    0.0, 1.0, 0.0, // second column
    -0.8*sin(time), 0.0, cos(time)  // third column
    );
    vec3 pos = mx * my * aPos;
    gl_Position = vec4(pos, 1.0);
    ourColor = aColor;
    texCoords = aTex;
}