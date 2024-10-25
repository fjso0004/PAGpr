#version 410

layout (location = 0) in vec3 posicion;

uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * vec4(posicion, 1.0);
}