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
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


namespace PAG
{
    // Inicialización del puntero estático
    Renderer* Renderer::instancia = nullptr;

    /**
    * Constructor por defecto
    */
    Renderer::Renderer() : shaderProgram(std::make_shared<ShaderProgram>()), camara(nullptr) {
        camara = new Camara(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f, 1024.0f / 576.0f, 0.1f, 100.0f);
        glEnable(GL_DEPTH_TEST); // Activa el test de profundidad en OpenGL
    }

    /**
    * Destructor
    */
    Renderer::~Renderer() {
        if (camara) {
            delete camara;
            camara = nullptr;
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
        glPolygonMode(GL_FRONT_AND_BACK, (modoActual == ModoVisualizacion::Alambre) ? GL_LINE : GL_FILL);

        if (shaderProgram) {
            shaderProgram->useProgram(); // Activar el programa de shaders
            setUniforms();
            // Configurar subrutinas según el modo actual
            GLuint activeSubroutine = (modoActual == ModoVisualizacion::Alambre) ? subroutineIndices[0] : subroutineIndices[1];
            glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &activeSubroutine);

            for (const auto& modelo : models) {
                modelo->renderizar(shaderProgram->getProgramID()); // Renderizar con la matriz de transformación actualizada
            }
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
    void PAG::Renderer::loadShaderProgram(const std::string& shaderBase) {
        shaderProgram = std::make_shared<ShaderProgram>();
        shaderProgram->loadShaders(shaderBase + "-vs.glsl", shaderBase + "-fs.glsl");

        // Obtener índices de las subrutinas
        subroutineIndices[0] = glGetSubroutineIndex(shaderProgram->getProgramID(), GL_FRAGMENT_SHADER, "colorModoAlambre");
        subroutineIndices[1] = glGetSubroutineIndex(shaderProgram->getProgramID(), GL_FRAGMENT_SHADER, "colorModoSolido");
    }

    void Renderer::setUniforms() {
        if (!shaderProgram) return;

        glm::mat4 view = camara->getViewMatrix();
        glm::mat4 projection = camara->getProjectionMatrix();

        shaderProgram->useProgram();
        GLint viewLoc = glGetUniformLocation(shaderProgram->getProgramID(), "view");
        GLint projLoc = glGetUniformLocation(shaderProgram->getProgramID(), "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    }

    Camara* Renderer::getCamara() const {
        return camara;
    }

    void Renderer::SetShaderProgram(std::shared_ptr<ShaderProgram> shader) {
        shaderProgram = shader;
    }

    void Renderer::cargarModelo(const std::string& filePath) {
        try {
            auto model = std::make_unique<ModeloOBJ>(filePath);
            if (model->cargarModelo()) {
                model->inicializarBuffers();
                models.push_back(std::move(model));
                std::cout << "Modelo cargado y buffers inicializados: " << filePath << std::endl;
            } else {
                std::cerr << "Error al cargar el modelo: " << filePath << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Excepción al cargar el modelo: " << e.what() << std::endl;
        }
    }

    void Renderer::eliminarModelo(int index) {
        if (index >= 0 && index < models.size()) {
            models.erase(models.begin() + index);
            std::cout << "Modelo eliminado en el índice: " << index << std::endl;
        }
    }

    void Renderer::renderizarEscena() {
        if (shaderProgram) {
            shaderProgram->useProgram();  // Activar el shader program
            for (const auto& modelo : models) {
                modelo->renderizar(shaderProgram->getProgramID());
            }
        }
    }

    void Renderer::actualizarTransformacion(int index, const glm::mat4& transform) {
        if (index >= 0 && index < models.size()) {
            models[index]->SetModelMatrix(transform);
        }
    }

    const std::shared_ptr<ShaderProgram> &Renderer::getShaderProgram() const {
        return shaderProgram;
    }

    void PAG::Renderer::cambiarModoVisualizacion(ModoVisualizacion nuevoModo) {
        modoActual = nuevoModo;
    }

    ModoVisualizacion PAG::Renderer::getModoVisualizacion() const {
        return modoActual;
    }
}
