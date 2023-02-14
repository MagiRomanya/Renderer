#version 330 core

in vec2 TexCoord;
in vec3 normal;

out vec4 FragColor;

void main()
{
    FragColor = abs(vec4(normal, 1.0));
}
