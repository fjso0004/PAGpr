//
// Created by admin on 20/09/2024.
//

#ifndef PAGPR_RENDERER_H
#define PAGPR_RENDERER_H

/**
* @file Renderer.h
* @author algarcia
*
* @date 2021/08/06
*
* @brief Declaración de la clase Renderer
*/

/**
* Espacio de nombres para las prácticas de Programación de Aplicaciones
* Gráficas
*/
#include "glad/glad.h"
#include <string>

namespace PAG
{
    /**
    * @brief Clase encargada de encapsular la gestión del área de dibujo
    * OpenGL
    *
    * Esta clase coordina el renderizado de las escenas OpenGL. Se implementa
    * aplicando el patrón de diseño Singleton. Está pensada para que las
    * funciones callback hagan llamadas a sus métodos
    */
    class Renderer
    {
    private:
        static Renderer* instancia; ///< Puntero al único objeto
        Renderer ();
        GLuint idVS = 0; // Identificador del vertex shader
        GLuint idFS = 0; // Identificador del fragment shader
        GLuint idSP = 0; // Identificador del shader program
        GLuint idVAO = 0; // Identificador del vertex array object
        GLuint idVBO = 0; // Identificador del vertex buffer object
        GLuint idIBO = 0; // Identificador del index buffer object

    public:
        virtual ~Renderer ();

        /**
        * @brief Consulta del objeto único de la clase
        * @return Referencia a la instancia única de la clase
        */
        static Renderer& getInstancia ();

        /**
        * @brief Método para hacer el refresco de la escena
        */
        void refrescar ();

        // Método para cambiar el tamaño del viewport
        void cambiarTamano(int width, int height);

        // Método para cambiar el color de fondo
        void cambiarColorFondo(float red, float green, float blue, float alpha);

        void creaShaderProgram( );

        void creaModelo ( );

        void loadShaderProgram(std::string archivo);

        GLuint loadShader(const std::string &fileName, GLenum shaderType);
    };
}


#endif //PAGPR_RENDERER_H
