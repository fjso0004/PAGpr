#version 410

layout (location = 0) in vec3 posicion;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec3 posicionV;
    vec3 normalV;
} salida;

void main() {
    vec4 posMundo = model * vec4(posicion, 1.0);
    salida.posicionV = vec3(view * posMundo);
    salida.normalV = mat3(view) * vec3(0.0, 0.0, 1.0); // Normal básica
    gl_Position = projection * view * posMundo;
}
