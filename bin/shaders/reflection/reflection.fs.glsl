#version 410 core
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform samplerCube skybox;

void main() {
    vec3 I = normalize(FragPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
