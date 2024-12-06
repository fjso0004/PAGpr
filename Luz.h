//
// Created by 34660 on 28/11/2024.
//

#ifndef PAGPR_LUZ_H
#define PAGPR_LUZ_H

#include <glm/glm.hpp>

enum class TipoLuz {
    Ambiente,
    Puntual,
    Direccional,
    Foco
};

struct Luz {
    TipoLuz tipo;
    glm::vec3 colorAmbiente;
    glm::vec3 colorDifusa;
    glm::vec3 colorEspecular;
    glm::vec3 posicion;
    glm::vec3 direccion;
    float anguloApertura;
    float atenuacion;
};

#endif //PAGPR_LUZ_H
