#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat;

void main() {
    vec3 fragPos = vec3(view * model * vec4(aPos, 1.0));
    vs_out.normal = normalMat * aNormal;
    gl_Position = projection * vec4(fragPos, 1.0);
}
