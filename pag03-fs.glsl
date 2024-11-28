#version 410

in vec3 posicionV;
in vec3 normalV;

uniform vec3 Ia, Id, Is; // Intensidades de luz ambiente, difusa y especular
uniform vec3 Ka, Kd, Ks; // Propiedades del material
uniform float ns;        // Exponente para el brillo especular

uniform vec3 luzPosicion;   // Para luces puntuales y focos
uniform vec3 luzDireccion;  // Para luces direccionales y focos
uniform float luzApertura;  // Ángulo de apertura para focos

uniform int tipoLuz;

subroutine vec4 CalculoLuz();
subroutine uniform CalculoLuz luzActiva;

subroutine(CalculoLuz)
vec4 luzAmbiente() {
    return vec4(Ia * Ka, 1.0);
}

subroutine(CalculoLuz)
vec4 luzPuntual() {
    vec3 L = normalize(luzPosicion - posicionV);
    vec3 N = normalize(normalV);
    vec3 V = normalize(-posicionV);
    vec3 R = reflect(-L, N);

    vec3 difusa = max(dot(N, L), 0.0) * Kd * Id;
    vec3 especular = pow(max(dot(R, V), 0.0), ns) * Ks * Is;

    return vec4(difusa + especular, 1.0);
}

// Implementa luz direccional y foco de manera similar.

void main() {
    gl_FragColor = luzActiva();
}