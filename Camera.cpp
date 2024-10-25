//
// Created by 34660 on 22/10/2024.
//

#include "Camera.h"

glm::mat4 Camara::getViewMatrix() {
    return glm::lookAt(posicion, lookAt, up);  // Calcula la matriz de visión
}

glm::mat4 Camara::getProjectionMatrix() {
    return glm::perspective(glm::radians(fovY), aspecto, zNear, zFar);  // Matriz de proyección
}

Camara::Camara(glm::vec3 pos, glm::vec3 look, glm::vec3 upDir, float fov, float aspect, float near, float far) : posicion(pos), lookAt(look), up(upDir), fovY(fov), aspecto(aspect), zNear(near), zFar(far){

}

glm::vec3 Camara::forward() const {
    return glm::normalize(lookAt - posicion);  // Normaliza la dirección hacia adelante
}

void Camara::pan(float angulo) {
    // Rotar el punto al que mira la cámara alrededor del eje "up" (eje v)
    glm::vec3 direccion = lookAt - posicion;  // Vector que va desde la posición de la cámara al punto de interés
    glm::mat4 rotacion = glm::rotate(glm::mat4(1.0f), glm::radians(angulo), up);  // Rotación alrededor del eje "up"
    direccion = glm::vec3(rotacion * glm::vec4(direccion, 1.0f));  // Aplicamos la rotación al vector dirección
    lookAt = posicion + direccion;  // Actualizamos el punto al que mira la cámara
}

void Camara::tilt(float angulo) {
// Calcula el vector lateral (eje u) de la cámara
    glm::vec3 u = glm::normalize(glm::cross(lookAt - posicion, up));
    glm::vec3 direccion = lookAt - posicion;  // Dirección desde la posición al punto de interés
    glm::mat4 rotacion = glm::rotate(glm::mat4(1.0f), glm::radians(angulo), u);  // Rotación alrededor del eje u
    direccion = glm::vec3(rotacion * glm::vec4(direccion, 1.0f));  // Aplicamos la rotación
    lookAt = posicion + direccion;  // Actualizamos el punto de interés
}

void Camara::dolly(float distancia) {
    glm::vec3 forward = glm::normalize(lookAt - posicion);  // Vector hacia adelante
    glm::vec3 desplazamiento = forward * distancia;
    posicion += desplazamiento;  // Mover la cámara
    lookAt += desplazamiento;    // Mover el punto de interés
}

void Camara::crane(float distancia) {
    glm::vec3 desplazamiento = glm::vec3(0.0f, distancia, 0.0f);  // Movimiento en el eje Y
    posicion += desplazamiento;  // Mover la cámara
    lookAt += desplazamiento;    // Mover el punto de interés
}

void Camara::orbit(float anguloLatitud, float anguloLongitud) {
    glm::vec3 direccion = posicion - lookAt;  // Vector que va desde el punto de interés a la cámara
    glm::vec3 u = glm::normalize(glm::cross(lookAt - posicion, up));  // Eje lateral (u)

    // Rotar alrededor de la latitud (eje u)
    glm::mat4 rotacionLat = glm::rotate(glm::mat4(1.0f), glm::radians(anguloLatitud), u);
    direccion = glm::vec3(rotacionLat * glm::vec4(direccion, 1.0f));

    // Rotar alrededor de la longitud (eje up)
    glm::mat4 rotacionLong = glm::rotate(glm::mat4(1.0f), glm::radians(anguloLongitud), up);
    direccion = glm::vec3(rotacionLong * glm::vec4(direccion, 1.0f));

    posicion = lookAt + direccion;  // Actualizamos la posición de la cámara
}

void Camara::zoom(float factor) {
    fovY = glm::clamp(fovY + factor, 1.0f, 45.0f);  // Limita el ángulo de visión entre 1° y 45°
}
