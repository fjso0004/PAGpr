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

#include <stdexcept>
#include <fstream>
#include <sstream>
#include "Renderer.h"
#include "glad/glad.h"

namespace PAG
{
    // Inicialización del puntero estático
    Renderer* Renderer::instancia = nullptr;

    /**
    * Constructor por defecto
    */
    Renderer::Renderer ()
    {
        glEnable(GL_DEPTH_TEST);
    }

    /**
    * Destructor
    */
    Renderer::~Renderer ()
    {
        if ( idVS != 0 )
        { glDeleteShader ( idVS );
        }
        if ( idFS != 0 )
        { glDeleteShader ( idFS );
        }
        if ( idSP != 0 )
        { glDeleteProgram ( idSP );
        }
        if ( idVBO != 0 )
        { glDeleteBuffers ( 1, &idVBO );
        }
        if ( idIBO != 0 )
        { glDeleteBuffers ( 1, &idIBO );
        }
        if ( idVAO != 0 )
        { glDeleteVertexArrays ( 1, &idVAO );
        }
    }

    /**
    * Consulta del objeto único de la clase
    * @return Referencia al objeto único de la clase
    */
    Renderer& Renderer::getInstancia ()
    {
        if (!instancia) // Lazy initialization
        {
            instancia = new Renderer;
        }
        return *instancia;
    }

    /**
    * Método para hacer el refresco de la escena
    */
    void Renderer::refrescar ()
    {
        glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
        glUseProgram ( idSP );
        glBindVertexArray ( idVAO );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );
        glDrawElements ( GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr );;
    }

    void Renderer::cambiarTamano(int width, int height) {
        // Ajusta el tamaño del viewport de OpenGL
        glViewport(0, 0, width, height);
    }

    void Renderer::cambiarColorFondo(float red, float green, float blue, float alpha) {
        // Cambia el color con el que se limpia el frame buffer
        glClearColor(red, green, blue, alpha);
    }

    /**
* Método para crear, compilar y enlazar el shader program
* @note No se incluye ninguna comprobación de errores
*/
/*
    void PAG::Renderer::creaShaderProgram( )
    { std::string miVertexShader =
                "#version 410\n"
                "layout (location = 0) in vec3 posicion;\n"
                "void main ()\n"
                "{ gl_Position = vec4 ( posicion, 1 );\n"
                "}\n";
        std::string miFragmentShader ="#version 410\n"
                                      "out vec4 colorFragmento;\n"
                                      "void main ()\n"
                                      "{ colorFragmento = vec4 ( 1.0, .4, .2, 1.0 );\n"
                                      "}\n";
        idVS = glCreateShader ( GL_VERTEX_SHADER );
        const GLchar* fuenteVS = miVertexShader.c_str ();
        glShaderSource ( idVS, 1, &fuenteVS, nullptr );
        glCompileShader ( idVS );
        idFS = glCreateShader ( GL_FRAGMENT_SHADER );
        const GLchar* fuenteFS = miFragmentShader.c_str ();
        glShaderSource ( idFS, 1, &fuenteFS, nullptr );
        glCompileShader ( idFS );
        idSP = glCreateProgram ();
        glAttachShader ( idSP, idVS );
        glAttachShader ( idSP, idFS );
        glLinkProgram ( idSP );
        GLint resultadoEnlazado = 0;
        glGetProgramiv(idSP, GL_LINK_STATUS, &resultadoEnlazado);
        if (resultadoEnlazado == GL_FALSE) {  // Ha habido un error
            GLint tamMsj = 0;
            std::string mensaje = "Error al enlazar el shader program: ";
            glGetProgramiv(idSP, GL_INFO_LOG_LENGTH, &tamMsj);
            if (tamMsj > 0) {
                GLchar* mensajeFormatoC = new GLchar[tamMsj];
                GLint datosEscritos = 0;
                glGetProgramInfoLog(idSP, tamMsj, &datosEscritos, mensajeFormatoC);
                mensaje += mensajeFormatoC;
                delete[] mensajeFormatoC;
                mensajeFormatoC = nullptr;
            }

            // Lanza la excepción con el mensaje de error
            throw std::runtime_error(mensaje);
        }
    }
    */

    /**
* Método para crear el VAO para el modelo a renderizar
* @note No se incluye ninguna comprobación de errores
*/
    void PAG::Renderer::creaModelo ( )
    { GLfloat vertices[] = { -.5, -.5, 0,
                             .5, -.5, 0,
                             .0, .5, 0 };
        GLuint indices[] = { 0, 1, 2 };
        glGenVertexArrays ( 1, &idVAO );
        glBindVertexArray ( idVAO );glGenBuffers ( 1, &idVBO );
        glBindBuffer ( GL_ARRAY_BUFFER, idVBO );
        glBufferData ( GL_ARRAY_BUFFER, 9*sizeof(GLfloat), vertices,
                       GL_STATIC_DRAW );
        glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat),
                                nullptr );
        glEnableVertexAttribArray ( 0 );
        glGenBuffers ( 1, &idIBO );
        glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, idIBO );
        glBufferData ( GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(GLuint), indices,
                       GL_STATIC_DRAW );
        GLint resultadoEnlazado = 0;
        glGetProgramiv(idSP, GL_LINK_STATUS, &resultadoEnlazado);
        if (resultadoEnlazado == GL_FALSE) {  // Ha habido un error
            GLint tamMsj = 0;
            std::string mensaje = "Error al enlazar el shader program: ";
            glGetProgramiv(idSP, GL_INFO_LOG_LENGTH, &tamMsj);
            if (tamMsj > 0) {
                GLchar* mensajeFormatoC = new GLchar[tamMsj];
                GLint datosEscritos = 0;
                glGetProgramInfoLog(idSP, tamMsj, &datosEscritos, mensajeFormatoC);
                mensaje += mensajeFormatoC;
                delete[] mensajeFormatoC;
                mensajeFormatoC = nullptr;
            }

            // Lanza la excepción con el mensaje de error
            throw std::runtime_error(mensaje);
        }
    }

    void Renderer::loadShaderProgram(std::string archivo) {
        // Cargar y compilar los shaders desde los archivos
        idVS = loadShader(archivo + "-vs.glsl", GL_VERTEX_SHADER);
        idFS = loadShader(archivo + "-fs.glsl", GL_FRAGMENT_SHADER);

        // Crear el programa de shaders
        idSP = glCreateProgram();
        glAttachShader(idSP, idVS);
        glAttachShader(idSP, idFS);
        glLinkProgram(idSP);

        // Verificar errores de enlazado
        GLint linkStatus = 0;
        glGetProgramiv(idSP, GL_LINK_STATUS, &linkStatus);
        if (linkStatus == GL_FALSE) {
            GLint logLength = 0;
            glGetProgramiv(idSP, GL_INFO_LOG_LENGTH, &logLength);

            std::string logMessage(logLength, ' ');
            glGetProgramInfoLog(idSP, logLength, nullptr, &logMessage[0]);

            throw std::runtime_error("Error al enlazar el shader program: " + logMessage);
        }
    }


    GLuint Renderer::loadShader(const std::string& fileName, GLenum shaderType) {
        // Leer el archivo de shader
        std::ifstream shaderFile(fileName);
        if (!shaderFile.is_open()) {
            throw std::runtime_error("No se pudo abrir el archivo de shader: " + fileName);
        }

        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();

        std::string shaderCode = shaderStream.str();
        const char* shaderCodeCStr = shaderCode.c_str();

        // Crear y compilar el shader
        GLuint shaderID = glCreateShader(shaderType);
        glShaderSource(shaderID, 1, &shaderCodeCStr, nullptr);
        glCompileShader(shaderID);

        // Verificar errores de compilación
        GLint compileStatus = 0;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
        if (compileStatus == GL_FALSE) {
            GLint logLength = 0;
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);

            std::string logMessage(logLength, ' ');
            glGetShaderInfoLog(shaderID, logLength, nullptr, &logMessage[0]);

            throw std::runtime_error("Error al compilar el shader " + fileName + ": " + logMessage);
        }

        return shaderID;
    }

}

