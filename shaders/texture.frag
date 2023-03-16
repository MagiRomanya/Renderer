#version 330 core

in vec2 TexCoord;
in vec3 normal;
in vec3 fragPos;

uniform sampler2D gandalf;

out vec4 FragColor;

vec3 lightPos = vec3(0, 10, -5);
vec3 lightColor = vec3(1, 1, 1);
const float diffuseIntensity = 1;
vec3 ambientLight = vec3(0.1, 0.1, 0.1);

void main()
{
    vec3 norm = normalize(normal);

    vec3 light_frag_vec = lightPos - fragPos;
    float light_frag_dist2 = dot(light_frag_vec, light_frag_vec);
    vec3 lightDir = normalize(light_frag_vec);

    vec3 diffuse = diffuseIntensity * max(dot(norm, lightDir), 0.0) * lightColor;

    FragColor = vec4(ambientLight + diffuse, 1) * texture(gandalf, TexCoord);
    // FragColor = vec4(max(dot(norm, lightDir), 0.0), 0, 0, 1);
}
