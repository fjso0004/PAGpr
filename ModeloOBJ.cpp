//
// Created by 34660 on 26/11/2024.
//

// Actualizado: ModeloOBJ con soporte para Normal Mapping
#include "ModeloOBJ.h"
#include <glm/gtc/type_ptr.hpp>

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

            glm::vec3 positions[3];
            glm::vec2 texCoords[3];

            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shape.mesh.indices[indexOffset + v];
                glm::vec3 vertexPosition = glm::vec3(
                        attrib.vertices[3 * idx.vertex_index + 0],
                        attrib.vertices[3 * idx.vertex_index + 1],
                        attrib.vertices[3 * idx.vertex_index + 2]
                );

                glm::vec3 vertexNormal = glm::vec3(0.0f);
                if (idx.normal_index >= 0) {
                    vertexNormal = glm::vec3(
                            attrib.normals[3 * idx.normal_index + 0],
                            attrib.normals[3 * idx.normal_index + 1],
                            attrib.normals[3 * idx.normal_index + 2]
                    );
                }

                glm::vec2 vertexTexCoord = glm::vec2(0.0f);
                if (idx.texcoord_index >= 0) {
                    vertexTexCoord = glm::vec2(
                            attrib.texcoords[2 * idx.texcoord_index + 0],
                            attrib.texcoords[2 * idx.texcoord_index + 1]
                    );
                }

                positions[v] = vertexPosition;
                texCoords[v] = vertexTexCoord;

                vertices.push_back({ vertexPosition, vertexNormal, vertexTexCoord });
                indices.push_back(static_cast<unsigned int>(indices.size()));
            }

            // Calcular tangentes y bitangentes
            glm::vec3 edge1 = positions[1] - positions[0];
            glm::vec3 edge2 = positions[2] - positions[0];

            glm::vec2 deltaUV1 = texCoords[1] - texCoords[0];
            glm::vec2 deltaUV2 = texCoords[2] - texCoords[0];

            float scaleFactor = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            glm::vec3 tangent = scaleFactor * (deltaUV2.y * edge1 - deltaUV1.y * edge2);
            glm::vec3 bitangent = scaleFactor * (-deltaUV2.x * edge1 + deltaUV1.x * edge2);

            float denominator = (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
            if (abs(denominator) < 1e-6) {
                tangent = glm::vec3(0.0f);
                bitangent = glm::vec3(0.0f);
            } else {
                float scaleFactor = 1.0f / denominator;
                tangent = scaleFactor * (deltaUV2.y * edge1 - deltaUV1.y * edge2);
                bitangent = scaleFactor * (-deltaUV2.x * edge1 + deltaUV1.x * edge2);
            }

            // Normalizar y asignar tangentes y bitangentes a los vértices
            for (int i = 0; i < 3; i++) {
                vertices[indices[indexOffset + i]].tangent += tangent;
                vertices[indices[indexOffset + i]].bitangent += bitangent;
            }

            indexOffset += fv;
        }
    }

    // Normalizar tangentes y bitangentes
    for (auto& vertex : vertices) {
        vertex.tangent = glm::normalize(vertex.tangent);
        vertex.bitangent = glm::normalize(vertex.bitangent);
    }

    std::cout << "Modelo cargado exitosamente: " << ruta << std::endl;
    return true;
}

void ModeloOBJ::inicializarBuffers() {
    liberarBuffers();

    glGenVertexArrays(1, &idVAO);
    glBindVertexArray(idVAO);

    glGenBuffers(1, &idVBO);
    glBindBuffer(GL_ARRAY_BUFFER, idVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &idIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
    glEnableVertexAttribArray(4);

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

    if (material.texturaID) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, material.texturaID);
        glUniform1i(glGetUniformLocation(shaderProgramID, "textura"), 0);
    }

    glBindVertexArray(idVAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    if (material.texturaID) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

const glm::mat4 &ModeloOBJ::getModelMatrix() const {
    return transformacion;
}
