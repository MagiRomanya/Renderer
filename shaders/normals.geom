#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

const float LENGTH = 0.5;

const float delta = LENGTH / 50.0; // minor displacement to avoid seeing normals from behind

uniform mat4 proj;

void generateVector(int index){
    gl_Position = proj * (gl_in[index].gl_Position+ vec4(gs_in[index].normal, 0.0) * delta); // base of the vector
    EmitVertex();
    gl_Position = proj * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * LENGTH); // top of the vector
    EmitVertex();
    EndPrimitive();
}

void main(){
    generateVector(0);
    generateVector(1);
    generateVector(2);
}
