#version 330 core

layout (location = 0) in vec2 model_coefficients;
layout (location = 1) in vec4 colors;

uniform mat4 model;

out vec4 vertex_colors;

void main() {
    gl_Position = model * vec4(model_coefficients.x, model_coefficients.y, 0.0, 1.0);
    vertex_colors = colors;
}
