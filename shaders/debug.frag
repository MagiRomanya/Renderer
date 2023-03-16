#version 330 core

in vec3 pos;

out vec4 FragColor;

void main() {
    vec2 color = pos.xy/2.0 + 0.5;
    FragColor = vec4(vec3(pos.y),1.0);
    // FragColor = vec4(1);
}
