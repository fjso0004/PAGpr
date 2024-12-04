#version 410

in vec3 posicionV; // Posición en espacio de visión
in vec3 normalV;   // Normal en espacio de visión

// Uniformes del material
uniform vec3 Ka;        // Color ambiente
uniform vec3 Kd;        // Color difuso
uniform vec3 Ks;        // Color especular
uniform float ns;       // Exponente especular

// Uniformes para la luz
uniform vec3 Ia;        // Intensidad de luz ambiente
uniform vec3 Id;        // Intensidad de luz difusa
uniform vec3 Is;        // Intensidad de luz especular
uniform vec3 luzPosicion; // Posición de la luz en espacio de visión

out vec4 FragColor;

void main() {
    // Dirección hacia la luz
    vec3 L = normalize(luzPosicion - posicionV);
    vec3 N = normalize(normalV);          // Normal del fragmento
    vec3 V = normalize(-posicionV);       // Vector hacia la cámara
    vec3 R = reflect(-L, N);              // Vector reflejado

    // Componentes de la iluminación
    vec3 ambiente = Ka * Ia;                                      // Luz ambiente
    vec3 difusa = max(dot(N, L), 0.0) * Kd * Id;                  // Luz difusa
    vec3 especular = pow(max(dot(R, V), 0.0), ns) * Ks * Is;      // Luz especular

    // Combinar componentes
    vec3 colorFinal = ambiente + difusa + especular;
    FragColor = vec4(colorFinal, 1.0);
}