#version 410

in VS_OUT {
    vec3 posicionV;
    vec3 normalV;
} entrada;

out vec4 colorFragmento;

uniform vec3 colorFijo; // Para el modo de alambre
uniform vec3 colorDifuso; // Color difuso del material

subroutine vec4 SeleccionColor();
subroutine uniform SeleccionColor metodoColor;

subroutine(SeleccionColor)
vec4 colorModoAlambre() {
    return vec4(colorFijo, 1.0);
}

subroutine(SeleccionColor)
vec4 colorModoSolido() {
    return vec4(colorDifuso, 1.0);
}

void main() {
    colorFragmento = metodoColor();
}
