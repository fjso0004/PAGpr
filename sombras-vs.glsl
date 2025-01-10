#version 410

layout (location = 0) in vec3 posicion;

uniform mat4 matrizSombras;
uniform mat4 model;

void main() {
    gl_Position = matrizSombras * model * vec4(posicion, 1.0);
}