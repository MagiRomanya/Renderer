#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;
layout (location = 3) in vec3 vColor;

uniform mat4 transform;

out vec3 color;

void main()
{
    color = vColor;
    gl_Position = transform * vec4(vPosition, 1.0);
}
