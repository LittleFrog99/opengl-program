#version 410 core
precision highp float;
layout (location = 0) in vec2 aPos;

out vec2 FragPos;

void main() {
    FragPos = aPos;
    gl_Position = vec4(aPos, 0, 1);
}
