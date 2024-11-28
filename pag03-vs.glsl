#version 410

layout (location = 0) in vec3 posicion;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 posicionV;
out vec3 normalV;

void main() {
    vec4 posMundo = model * vec4(posicion, 1.0);
    posicionV = vec3(view * posMundo);
    normalV = mat3(transpose(inverse(view * model))) * normal;

    gl_Position = projection * view * posMundo;
}