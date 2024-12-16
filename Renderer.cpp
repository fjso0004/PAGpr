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
        glEnable(GL_DEPTH_TEST);
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
        if (!instancia) {
            instancia = new Renderer;
        }
        return *instancia;
    }

    /**
 * Método para refrescar la escena
 */
    void Renderer::refrescar() {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (shaderProgram) {
            shaderProgram->useProgram();
            setUniforms();

            // Configurar el modo de polígonos
            //glPolygonMode(GL_FRONT_AND_BACK, (modoActual == ModoVisualizacion::Alambre) ? GL_LINE : GL_FILL);

            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);

            for (size_t i = 0; i < luces.size(); ++i) {
                const auto& luz = luces[i];
                if (i == 0) {
                    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                } else {
                    glBlendFunc(GL_ONE, GL_ONE);
                }

                setLuzUniforms(luz);

                GLuint subroutineIndex;
                switch (luz.tipo) {
                    case TipoLuz::Ambiente:
                        subroutineIndex = glGetSubroutineIndex(shaderProgram->getProgramID(), GL_FRAGMENT_SHADER, "luzAmbiente");
                        break;
                    case TipoLuz::Puntual:
                        subroutineIndex = glGetSubroutineIndex(shaderProgram->getProgramID(), GL_FRAGMENT_SHADER, "luzPuntual");
                        break;
                    case TipoLuz::Direccional:
                        subroutineIndex = glGetSubroutineIndex(shaderProgram->getProgramID(), GL_FRAGMENT_SHADER, "luzDireccional");
                        break;
                    case TipoLuz::Foco:
                        subroutineIndex = glGetSubroutineIndex(shaderProgram->getProgramID(), GL_FRAGMENT_SHADER, "luzFoco");
                        break;
                    default:
                        subroutineIndex = GL_INVALID_INDEX;
                        break;
                }

                if (subroutineIndex != GL_INVALID_INDEX) {
                    GLint numActiveSubroutines;
                    glGetProgramStageiv(shaderProgram->getProgramID(), GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &numActiveSubroutines);
                    std::vector<GLuint> subroutineIndices(numActiveSubroutines, GL_INVALID_INDEX);
                    subroutineIndices[0] = subroutineIndex; // Configura solo la subrutina activa
                    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, numActiveSubroutines, subroutineIndices.data());
                } else {
                    std::cerr << "Error: Subrutina no válida para tipo de luz: " << static_cast<int>(luz.tipo) << std::endl;
                }

                // Renderizar cada modelo
                for (const auto& modelo : models) {
                    Material material = modelo->getMaterial();

                    glUniform3fv(glGetUniformLocation(shaderProgram->getProgramID(), "Ka"), 1, glm::value_ptr(material.colorAmbiente));
                    glUniform3fv(glGetUniformLocation(shaderProgram->getProgramID(), "Kd"), 1, glm::value_ptr(material.colorDifuso));
                    glUniform3fv(glGetUniformLocation(shaderProgram->getProgramID(), "Ks"), 1, glm::value_ptr(material.colorEspecular));
                    glUniform1f(glGetUniformLocation(shaderProgram->getProgramID(), "ns"), 32.0f);

                    // Activar la textura si está disponible
                    GLuint usaTexturaLoc = glGetUniformLocation(shaderProgram->getProgramID(), "usaTextura");
                    if (material.texturaID) {
                        glUniform1i(usaTexturaLoc, GL_TRUE);
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, material.texturaID);
                        glUniform1i(glGetUniformLocation(shaderProgram->getProgramID(), "textura"), 0);
                    } else {
                        glUniform1i(usaTexturaLoc, GL_FALSE);
                    }

                    modelo->renderizar(shaderProgram->getProgramID());
                }
            }
            glDisable(GL_BLEND);
        }
    }

    /**
    * Método para cambiar el tamaño del viewport
    */
    void Renderer::cambiarTamano(int width, int height) {
        glViewport(0, 0, width, height);
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

    void Renderer::cargarModelo(const std::string& filePath, const std::string& texturaPath) {
        try {
            auto model = std::make_unique<ModeloOBJ>(filePath);
            if (model->cargarModelo()) {
                model->inicializarBuffers();
                models.push_back(std::move(model));

                // Cargar y asociar textura
                GLuint texturaID = cargarTextura(texturaPath);
                models.back()->setMaterial({
                                                   glm::vec3(0.8f, 0.5f, 0.3f),
                                                   glm::vec3(0.2f, 0.2f, 0.2f),
                                                   glm::vec3(1.0f, 1.0f, 1.0f),
                                                   texturaID
                                           });

                std::cout << "Modelo y textura cargados: " << filePath << " | " << texturaPath << std::endl;
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

    void Renderer::actualizarTransformacion(int index, const glm::mat4& transform) {
        if (index >= 0 && index < models.size()) {
            models[index]->SetModelMatrix(transform);
        }
    }

    /*
    void PAG::Renderer::cambiarModoVisualizacion(ModoVisualizacion nuevoModo) {
        modoActual = nuevoModo;
    }

    ModoVisualizacion PAG::Renderer::getModoVisualizacion() const {
        return modoActual;
    }
    */

    void Renderer::addLuz(const Luz& luz) {
        luces.push_back(luz);
    }

    void Renderer::clearLuces() {
        luces.clear();
    }

    void Renderer::setLuzUniforms(const Luz& luz) const {
        glUniform3fv(glGetUniformLocation(shaderProgram->getProgramID(), "Ia"), 1, glm::value_ptr(luz.colorAmbiente));
        glUniform3fv(glGetUniformLocation(shaderProgram->getProgramID(), "Id"), 1, glm::value_ptr(luz.colorDifusa));
        glUniform3fv(glGetUniformLocation(shaderProgram->getProgramID(), "Is"), 1, glm::value_ptr(luz.colorEspecular));

        if (luz.tipo == TipoLuz::Puntual || luz.tipo == TipoLuz::Foco) {
            glUniform3fv(glGetUniformLocation(shaderProgram->getProgramID(), "luzPosicion"), 1, glm::value_ptr(luz.posicion));
        }

        if (luz.tipo == TipoLuz::Direccional || luz.tipo == TipoLuz::Foco) {
            glUniform3fv(glGetUniformLocation(shaderProgram->getProgramID(), "luzDireccion"), 1, glm::value_ptr(luz.direccion));
            if (luz.tipo == TipoLuz::Foco) {
                glUniform1f(glGetUniformLocation(shaderProgram->getProgramID(), "luzApertura"), glm::radians(luz.anguloApertura));
            }
        }
    }

    std::vector<Luz>& Renderer::getLuces() {
        return luces;
    }

    GLuint Renderer::cargarTextura(const std::string& ruta) {
        std::vector<unsigned char> imagen;
        unsigned ancho, alto;
        unsigned error = lodepng::decode(imagen, ancho, alto, ruta.c_str());
        if (error) {
            throw std::runtime_error("Error cargando textura: " + std::string(lodepng_error_text(error)));
        }

        // Voltear la imagen verticalmente
        size_t filaBytes = ancho * 4;
        unsigned char* imgPtr = imagen.data();
        for (size_t y = 0; y < alto / 2; ++y) {
            std::swap_ranges(imgPtr + y * filaBytes, imgPtr + (y + 1) * filaBytes, imgPtr + (alto - y - 1) * filaBytes);
        }

        // Generar textura OpenGL
        GLuint texturaID;
        glGenTextures(1, &texturaID);
        glBindTexture(GL_TEXTURE_2D, texturaID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ancho, alto, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagen.data());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindTexture(GL_TEXTURE_2D, 0);
        return texturaID;
    }
}
