#version 330 core

out vec4 color;

in vec4 vertex_colors;

void main() {
    color = vertex_colors;
}
