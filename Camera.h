//
// Created by 34660 on 22/10/2024.
//

#ifndef PAGPR_CAMERA_H
#define PAGPR_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camara {
public:
    glm::vec3 posicion;  // Posición de la cámara
    glm::vec3 lookAt;    // Punto al que mira la cámara
    glm::vec3 up;        // Vector de arriba
    float fovY;          // Ángulo de visión vertical
    float aspecto;       // Relación de aspecto
    float zNear, zFar;   // Planos Z near y Z far

    Camara(glm::vec3 pos, glm::vec3 look, glm::vec3 upDir, float fov, float aspect, float near, float far);

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();
    glm::vec3 forward() const;

    void pan(float angulo);
    void tilt(float angulo);
    void dolly(float distancia);
    void crane(float distancia);
    void orbit(float anguloLatitud, float anguloLongitud);
    void zoom(float factor);

};


#endif //PAGPR_CAMERA_H
