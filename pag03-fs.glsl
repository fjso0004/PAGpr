#version 410

in VS_OUT {
    vec3 posicionV;
    vec3 normalV; // Debemos declarar esto, aunque no lo usemos
} entrada;

out vec4 colorFragmento;

uniform vec3 colorFijo;    // Color para el modo alambre
uniform vec3 colorDifuso;  // Color difuso para el modo sólido

subroutine vec4 SeleccionColor();
subroutine uniform SeleccionColor metodoColor;

subroutine(SeleccionColor)
vec4 colorModoAlambre() {
    return vec4(colorFijo, 1.0); // Fijo, rojo por ejemplo
}

subroutine(SeleccionColor)
vec4 colorModoSolido() {
    return vec4(colorDifuso, 1.0); // Usa el color difuso del material
}

void main() {
    colorFragmento = metodoColor(); // Llama a la subrutina activa
}
