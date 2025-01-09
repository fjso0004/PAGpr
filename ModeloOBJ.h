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

struct Material {
    glm::vec3 colorDifuso;
    glm::vec3 colorAmbiente;
    glm::vec3 colorEspecular;
    GLuint texturaID = 0;
    GLuint normalMapID = 0;
};

class ModeloOBJ {
public:
    //Vértices
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };

    ModeloOBJ(const std::string& rutaArchivo);
    ~ModeloOBJ();
    bool cargarModelo();
    void inicializarBuffers();
    void renderizar(GLuint shaderProgramID) const;
    void SetModelMatrix(const glm::mat4& nuevaTransformacion);
    const glm::mat4 &getModelMatrix() const;
    void setMaterial(const Material& nuevoMaterial) {material = nuevoMaterial;}
    Material& getMaterial() { return material; } // Devuelve referencia no constante
    const Material& getMaterial() const { return material; } // Devuelve referencia constante para objetos constantes

    void setNormalMap(GLuint normalMap) { material.normalMapID = normalMap; }
    GLuint getNormalMap() const { return material.normalMapID; }

private:
    std::string ruta;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;//Índices del modelo

    // Identificadores de los buffers de OpenGL
    GLuint idVAO;
    GLuint idVBO;
    GLuint idIBO;

    glm::mat4 transformacion = glm::mat4(1.0f);

    void liberarBuffers();
    Material material;
};

#endif // MODELO_OBJ_H

