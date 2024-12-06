#version 410

in vec3 posicionV; // Posición en espacio de visión
in vec3 normalV;   // Normal en espacio de visión

// Uniformes del material
uniform vec3 Ka;        // Color ambiente
uniform vec3 Kd;        // Color difuso
uniform vec3 Ks;        // Color especular
uniform float ns;       // Exponente especular

// Uniformes de la luz
uniform vec3 Ia;        // Intensidad de luz ambiente
uniform vec3 Id;        // Intensidad de luz difusa
uniform vec3 Is;        // Intensidad de luz especular
uniform vec3 luzPosicion; // Posición de la luz en espacio de visión
uniform vec3 luzDireccion; // Dirección de la luz
uniform float luzApertura; // Ángulo de apertura (para foco)

out vec4 FragColor;

// Subrutinas para diferentes tipos de luz
subroutine vec4 LuzCalculationType();
subroutine uniform LuzCalculationType luzActiva;

// Subrutina para luz ambiente
subroutine(LuzCalculationType)
vec4 luzAmbiente() {
    vec3 ambiente = Ka * Ia;
    return vec4(ambiente, 1.0);
}

// Subrutina para luz puntual
subroutine(LuzCalculationType)
vec4 luzPuntual() {
    vec3 L = normalize(luzPosicion - posicionV);
    vec3 N = normalize(normalV);
    vec3 V = normalize(-posicionV);
    vec3 R = reflect(-L, N);

    vec3 ambiente = Ka * Ia;
    vec3 difusa = max(dot(N, L), 0.0) * Kd * Id;
    vec3 especular = pow(max(dot(R, V), 0.0), ns) * Ks * Is;

    return vec4(ambiente + difusa + especular, 1.0);
}

// Subrutina para luz direccional
subroutine(LuzCalculationType)
vec4 luzDireccional() {
    vec3 L = normalize(-luzDireccion);
    vec3 N = normalize(normalV);
    vec3 V = normalize(-posicionV);
    vec3 R = reflect(-L, N);

    vec3 ambiente = Ka * Ia;
    vec3 difusa = max(dot(N, L), 0.0) * Kd * Id;
    vec3 especular = pow(max(dot(R, V), 0.0), ns) * Ks * Is;

    return vec4(ambiente + difusa + especular, 1.0);
}

// Subrutina para foco de luz
subroutine(LuzCalculationType)
vec4 luzFoco() {
    vec3 L = normalize(luzPosicion - posicionV);
    vec3 N = normalize(normalV);
    vec3 V = normalize(-posicionV);
    vec3 R = reflect(-L, N);

    // Ángulo entre la dirección de la luz y la dirección del foco
    float anguloLuz = dot(-L, normalize(luzDireccion));
    if (anguloLuz < cos(radians(luzApertura))) {
        return vec4(Ka * Ia, 1.0); // Fuera del cono, solo componente ambiente
    }

    vec3 ambiente = Ka * Ia;
    vec3 difusa = max(dot(N, L), 0.0) * Kd * Id;
    vec3 especular = pow(max(dot(R, V), 0.0), ns) * Ks * Is;

    return vec4(ambiente + difusa + especular, 1.0);
}

void main() {
    FragColor = luzActiva(); // Selecciona la subrutina activa
}