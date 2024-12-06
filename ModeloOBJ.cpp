//
// Created by 34660 on 26/11/2024.
//

#include "ModeloOBJ.h"
#include "glm/gtc/type_ptr.hpp"

ModeloOBJ::ModeloOBJ(const std::string& rutaArchivo)
        : ruta(rutaArchivo), idVAO(0), idVBO(0), idIBO(0) {}

ModeloOBJ::~ModeloOBJ() {
    liberarBuffers();
}

bool ModeloOBJ::cargarModelo() {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    // Cargar el archivo OBJ
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, ruta.c_str());

    if (!warn.empty()) {
        std::cout << "Warning: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << "Error: " << err << std::endl;
    }

    if (!ret) {
        std::cerr << "Fallo al cargar el modelo OBJ: " << ruta << std::endl;
        return false;
    }

    for (const auto& shape : shapes) {
        size_t indexOffset = 0;

        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            int fv = shape.mesh.num_face_vertices[f];

            // Procesar cada vértice del polígono
            for (size_t v = 0; v < fv; v++) {
                // Índices del vértice
                tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];
                glm::vec3 vertexPosition = glm::vec3(
                        attrib.vertices[3 * idx.vertex_index + 0],
                        attrib.vertices[3 * idx.vertex_index + 1],
                        attrib.vertices[3 * idx.vertex_index + 2]
                );

                glm::vec3 vertexNormal = glm::vec3(0.0f, 0.0f, 0.0f);
                if (idx.normal_index >= 0) {
                    vertexNormal = glm::vec3(
                            attrib.normals[3 * idx.normal_index + 0],
                            attrib.normals[3 * idx.normal_index + 1],
                            attrib.normals[3 * idx.normal_index + 2]
                    );
                }

                glm::vec2 vertexTexCoord = glm::vec2(0.0f, 0.0f);
                if (idx.texcoord_index >= 0) {
                    vertexTexCoord = glm::vec2(
                            attrib.texcoords[2 * idx.texcoord_index + 0],
                            attrib.texcoords[2 * idx.texcoord_index + 1]
                    );
                }

                vertices.push_back({ vertexPosition, vertexNormal, vertexTexCoord });
                indices.push_back(static_cast<unsigned int>(indices.size()));
            }
            indexOffset += fv;
        }
    }

    std::cout << "Modelo cargado exitosamente: " << ruta << std::endl;
    return true;
}

// Función para inicializar los buffers de OpenGL
void ModeloOBJ::inicializarBuffers() {
    // Liberar buffers si ya existían
    liberarBuffers();

    // Generación y configuración del VAO
    glGenVertexArrays(1, &idVAO);
    glBindVertexArray(idVAO);

    // Generación y configuración del VBO (Vertex Buffer Object)
    glGenBuffers(1, &idVBO);
    glBindBuffer(GL_ARRAY_BUFFER, idVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Generación y configuración del IBO (Index Buffer Object)
    glGenBuffers(1, &idIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Definición del formato de los vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    // Desenlazar el VAO
    glBindVertexArray(0);
}

void ModeloOBJ::liberarBuffers() {
    if (idVBO != 0) { glDeleteBuffers(1, &idVBO); idVBO = 0; }
    if (idIBO != 0) { glDeleteBuffers(1, &idIBO); idIBO = 0; }
    if (idVAO != 0) { glDeleteVertexArrays(1, &idVAO); idVAO = 0; }
}

void ModeloOBJ::SetModelMatrix(const glm::mat4 &nuevaTransformacion) {
    transformacion = nuevaTransformacion;
}

void ModeloOBJ::renderizar(GLuint shaderProgramID) const {
    if (idVAO == 0) return;

    GLuint modelLoc = glGetUniformLocation(shaderProgramID, "model");
    if (modelLoc != -1) {
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transformacion));
    }

    glBindVertexArray(idVAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
