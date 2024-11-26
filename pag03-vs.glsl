#version 410

layout (location = 0) in vec3 posicion;

uniform mat4 model;       // Matriz de transformación del modelo
uniform mat4 view;        // Matriz de vista (cámara)
uniform mat4 projection;  // Matriz de proyección

void main() {
    gl_Position = projection * view * model * vec4(posicion, 1.0);
}
