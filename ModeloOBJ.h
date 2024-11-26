//
// Created by 34660 on 26/11/2024.
//

#ifndef MODELO_OBJ_H
#define MODELO_OBJ_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <tiny_obj_loader.h>
#include <vector>
#include <string>
#include <iostream>

class ModeloOBJ {
public:
    // Estructura para almacenar la información de un vértice
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    // Constructor
    ModeloOBJ(const std::string& rutaArchivo);

    // Destructor
    ~ModeloOBJ();

    // Función para cargar el archivo .obj
    bool cargarModelo();

    // Función para inicializar los buffers de OpenGL
    void inicializarBuffers();

    // Función para renderizar el modelo
    void renderizar(GLuint shaderProgramID) const;

    void SetModelMatrix(const glm::mat4& nuevaTransformacion);
    glm::mat4 GetModelMatrix();

private:
    std::string ruta;                     // Ruta del archivo .obj
    std::vector<Vertex> vertices;         // Vértices del modelo
    std::vector<unsigned int> indices;    // Índices del modelo

    // Identificadores de los buffers de OpenGL
    GLuint idVAO;
    GLuint idVBO;
    GLuint idIBO;

    glm::mat4 transformacion = glm::mat4(1.0f);

    // Función interna para liberar los recursos de OpenGL
    void liberarBuffers();
};

#endif // MODELO_OBJ_H

