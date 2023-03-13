#version 330 core

in vec2 TexCoord;

uniform sampler2D gandalf;

out vec4 FragColor;

void main()
{
    FragColor = texture(gandalf, TexCoord);
}
