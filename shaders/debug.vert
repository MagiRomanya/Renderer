#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;
layout (location = 3) in vec3 vColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 modelView;
uniform mat4 modelViewProj;
uniform mat4 normalMatrix;

out vec3 pos;

void main()
{
    vec4 position = modelViewProj * vec4(vPosition, 1.0);
    pos = position.xyz / position.w;
    vec4 pos_color = proj * view * model * vec4(vPosition, 1.0);
    pos = pos_color.xyz / position.w;
    gl_Position = position;
}
