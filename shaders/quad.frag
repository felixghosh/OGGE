#version 450 core
out vec4 FragColor;  

in vec3 ourColor;
in vec2 texCoords;

uniform sampler2D ourTexture;
  
void main()
{
    //FragColor = vec4(ourColor, 1.0);
    FragColor = texture(ourTexture, texCoords) * vec4(ourColor, 1.0f);
}