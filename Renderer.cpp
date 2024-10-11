//
// Created by admin on 20/09/2024.
//

/**
* @file Renderer.cpp
* @author algarcia
*
* @date 2021/08/06
*
* @brief Implementación de la clase Renderer
*/

#include "Renderer.h"
#include "ShaderProgram.h"
#include "glad/glad.h"

namespace PAG
{
    // Inicialización del puntero estático
    Renderer* Renderer::instancia = nullptr;

    /**
    * Constructor por defecto
    */
    Renderer::Renderer() : shaderProgram(nullptr) {
        glEnable(GL_DEPTH_TEST); // Activa el test de profundidad en OpenGL
    }

    /**
    * Destructor
    */
    Renderer::~Renderer() {
        if (shaderProgram) {
            delete shaderProgram; // Libera la memoria del programa de shaders
            shaderProgram = nullptr;
        }

        // Libera los recursos de OpenGL
        if (idVBO != 0) { glDeleteBuffers(1, &idVBO); }
        if (idIBO != 0) { glDeleteBuffers(1, &idIBO); }
        if (idVAO != 0) { glDeleteVertexArrays(1, &idVAO); }
    }

    /**
    * Consulta del objeto único de la clase
    * @return Referencia al objeto único de la clase
    */
    Renderer& Renderer::getInstancia() {
        if (!instancia) { // Lazy initialization
            instancia = new Renderer;
        }
        return *instancia;
    }

    /**
    * Método para refrescar la escena
    */
    void Renderer::refrescar() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpia los buffers
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Modo de dibujado

        if (shaderProgram) {
            shaderProgram->useProgram(); // Usa el shader program
        }

        glBindVertexArray(idVAO); // Asocia el VAO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO); // Asocia el IBO
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr); // Dibuja el triángulo
    }

    /**
    * Método para cambiar el tamaño del viewport
    */
    void Renderer::cambiarTamano(int width, int height) {
        glViewport(0, 0, width, height); // Ajusta el tamaño del viewport de OpenGL
    }

    /**
    * Método para cambiar el color de fondo
    */
    void Renderer::cambiarColorFondo(float red, float green, float blue, float alpha) {
        glClearColor(red, green, blue, alpha); // Cambia el color con el que se limpia el frame buffer
    }

    /**
    * Método para crear el VAO para el modelo a renderizar
    * @note No se incluye ninguna comprobación de errores
    */
    void Renderer::creaModelo() {
        // Definición de los vértices del triángulo
        GLfloat vertices[] = {
                -.5f, -.5f, 0.0f,
                0.5f, -.5f, 0.0f,
                0.0f,  0.5f, 0.0f
        };

        // Definición de los índices de los vértices
        GLuint indices[] = { 0, 1, 2 };

        // Generación y configuración del VAO
        glGenVertexArrays(1, &idVAO);
        glBindVertexArray(idVAO);

        // Generación y configuración del VBO (Vertex Buffer Object)
        glGenBuffers(1, &idVBO);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Definición del formato de los vértices
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0);

        // Generación y configuración del IBO (Index Buffer Object)
        glGenBuffers(1, &idIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    /**
    * Método para cargar el shader program
    * @param shaderBase Nombre base de los archivos de shaders
    */
    void Renderer::loadShaderProgram(const std::string& shaderBase) {
        if (shaderProgram) {
            delete shaderProgram; // Elimina el shader program anterior si existe
        }

        shaderProgram = new ShaderProgram(); // Crea un nuevo ShaderProgram
        shaderProgram->loadShaders(shaderBase + "-vs.glsl", shaderBase + "-fs.glsl"); // Carga los shaders
    }
}
