#version 410

layout (location = 0) in vec3 posicion;

uniform mat4 matrizModVisProy;

out vec4 coordenadasSombra; // Salida: Coordenadas de sombra

void main() {
    coordenadasSombra = matrizModVisProy * vec4(posicion, 1.0); // Calcula coordenadas en el espacio de la luz
    gl_Position = coordenadasSombra; // Para el mapa de sombras
}