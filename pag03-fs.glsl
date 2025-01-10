#version 410

in vec3 posicionV;
in vec2 texCoordF;
in mat3 TBN;
in vec4 coordenadasSombra;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float ns;

uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;
uniform vec3 luzPosicion;
uniform vec3 luzDireccion;
uniform float luzApertura;

uniform sampler2D textura;
uniform sampler2D normalMap;
uniform sampler2DShadow mapaSombras;
uniform bool usaTextura;

out vec4 FragColor;

subroutine vec4 LuzCalculationType();
subroutine uniform LuzCalculationType luzActiva;

float calcularSombra() {
    vec3 coordsSombra = coordenadasSombra.xyz / coordenadasSombra.w;
    coordsSombra = coordsSombra * 0.5 + 0.5;

    // Validar rango
    if (coordsSombra.x < 0.0 || coordsSombra.x > 1.0 ||
        coordsSombra.y < 0.0 || coordsSombra.y > 1.0) {
        return 1.0; // No hay sombra
    }
    return texture(mapaSombras, vec3(coordsSombra.xy, coordsSombra.z));
}

subroutine(LuzCalculationType)
vec4 luzPuntual() {
    vec3 L = normalize(luzPosicion - posicionV);
    vec3 N = normalize(TBN * (texture(normalMap, texCoordF).rgb * 2.0 - 1.0));
    vec3 V = normalize(-posicionV);
    vec3 R = reflect(-L, N);

    vec3 ambiente = Ka * Ia;
    vec3 difusa = max(dot(N, L), 0.0) * Kd * Id;
    vec3 especular = pow(max(dot(R, V), 0.0), ns) * Ks * Is;

    float sombra = calcularSombra();
    return vec4((ambiente + sombra * (difusa + especular)), 1.0);
}

subroutine(LuzCalculationType)
vec4 luzDireccional() {
    vec3 L = normalize(-luzDireccion);
    vec3 N = normalize(TBN * (texture(normalMap, texCoordF).rgb * 2.0 - 1.0));
    vec3 V = normalize(-posicionV);
    vec3 R = reflect(-L, N);

    vec3 ambiente = Ka * Ia;
    vec3 difusa = max(dot(N, L), 0.0) * Kd * Id;
    vec3 especular = pow(max(dot(R, V), 0.0), ns) * Ks * Is;

    float sombra = calcularSombra();
    return vec4((ambiente + sombra * (difusa + especular)), 1.0);
}

subroutine(LuzCalculationType)
vec4 luzAmbiente() {
    vec3 ambiente = Ka * Ia;
    return vec4(ambiente, 1.0);
}

subroutine(LuzCalculationType)
vec4 luzFoco() {
    vec3 L = normalize(luzPosicion - posicionV);
    vec3 N = normalize(TBN * (texture(normalMap, texCoordF).rgb * 2.0 - 1.0));
    vec3 V = normalize(-posicionV);
    vec3 R = reflect(-L, N);

    float anguloLuz = dot(-L, normalize(luzDireccion));
    if (anguloLuz < cos(radians(luzApertura))) {
        return vec4(Ka * Ia, 1.0);
    }

    vec3 ambiente = Ka * Ia;
    vec3 difusa = max(dot(N, L), 0.0) * Kd * Id;
    vec3 especular = pow(max(dot(R, V), 0.0), ns) * Ks * Is;

    return vec4(ambiente + difusa + especular, 1.0);
}

void main() {
    vec4 colorBase = luzActiva();

    if (usaTextura) {
        vec4 colorTextura = texture(textura, texCoordF);
        FragColor = vec4(colorBase.rgb * colorTextura.rgb, 1.0);
    } else {
        FragColor = colorBase;
    }
}