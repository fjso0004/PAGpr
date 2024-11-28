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
    glm::vec3 colorAmbiente;  // Ia
    glm::vec3 colorDifusa;    // Id
    glm::vec3 colorEspecular; // Is
    glm::vec3 posicion;       // p (si aplica)
    glm::vec3 direccion;      // d (si aplica)
    float anguloApertura;     // Ángulo del cono (gamma)
    float atenuacion;         // Atenuación (opcional)
};

#endif //PAGPR_LUZ_H
