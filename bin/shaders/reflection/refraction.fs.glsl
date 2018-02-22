#version 410 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform samplerCube skybox;

void main() {
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(FragPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
