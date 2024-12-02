#version 410

in vec3 posicionV;
in vec3 normalV;

uniform vec3 Ia, Id, Is; // Intensidades de luz
uniform vec3 Ka, Kd, Ks; // Propiedades del material
uniform float ns;        // Exponente especular
uniform vec3 luzPosicion;
uniform vec3 luzDireccion;
uniform float luzApertura;
uniform int tipoLuz;

subroutine vec4 CalculoLuz();
subroutine uniform CalculoLuz luzActiva;

// Subrutina para luz direccional
subroutine(CalculoLuz)
vec4 luzDireccional() {
    vec3 L = normalize(-luzDireccion);
    vec3 N = normalize(normalV);
    vec3 V = normalize(-posicionV);
    vec3 R = reflect(-L, N);

    vec3 difusa = max(dot(N, L), 0.0) * Kd * Id;
    vec3 especular = pow(max(dot(R, V), 0.0), ns) * Ks * Is;

    return vec4(difusa + especular, 1.0);
}

// Subrutina para foco de luz
subroutine(CalculoLuz)
vec4 luzFoco() {
    vec3 L = normalize(luzPosicion - posicionV);
    vec3 N = normalize(normalV);
    vec3 V = normalize(-posicionV);
    vec3 R = reflect(-L, N);

    // Ángulo entre L y la dirección del foco
    float anguloLuz = dot(-L, normalize(luzDireccion));
    if (anguloLuz < cos(luzApertura)) {
        return vec4(0.0); // Fuera del cono de luz
    }

    vec3 difusa = max(dot(N, L), 0.0) * Kd * Id;
    vec3 especular = pow(max(dot(R, V), 0.0), ns) * Ks * Is;

    return vec4(difusa + especular, 1.0);
}

void main() {
    gl_FragColor = luzActiva();
}