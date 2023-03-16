#version 330 core

in vec2 TexCoord;
in vec3 normal;
in vec3 color;

uniform sampler2D gandalf;

out vec4 FragColor;

void main()
{
    FragColor = vec4(color, 1);
}
