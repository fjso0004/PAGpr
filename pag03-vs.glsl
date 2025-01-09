#version 410

layout (location = 0) in vec3 posicion;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord; // Coordenadas de textura
layout (location = 3) in vec3 tangent;   // Tangente
layout (location = 4) in vec3 bitangent; // Bitangente

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 posicionV;       // Posición en espacio de visión
out vec2 texCoordF; // Salida de las coordenadas de textura
out mat3 TBN;             // Matriz TBN para transformar al espacio de la tangente

void main() {
    vec4 posMundo = model * vec4(posicion, 1.0);
    posicionV = vec3(view * posMundo);

    // Transformar tangente, bitangente y normal al espacio de visión
    vec3 T = normalize(mat3(view * model) * tangent);
    vec3 B = normalize(mat3(view * model) * bitangent);
    vec3 N = normalize(mat3(view * model) * normal);

    TBN = mat3(T, B, N); // Construir la matriz TBN

    texCoordF = texCoord; // Pasar las coordenadas de textura al fragment shader

    gl_Position = projection * view * posMundo;
}