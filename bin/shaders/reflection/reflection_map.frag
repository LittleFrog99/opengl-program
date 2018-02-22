#version 410 core
struct Material {
    sampler2D texture_height1;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform samplerCube skybox;
uniform Material material;

void main() {
    vec3 I = normalize(FragPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = texture(material.texture_height1, TexCoords) * texture(skybox, R);
}
