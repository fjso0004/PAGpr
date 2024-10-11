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
#include "ShaderProgram.h"
#include <string>

namespace PAG
{
    /**
    * @brief Clase encargada de encapsular la gestión del área de dibujo
    * OpenGL
    *
    * Esta clase coordina el renderizado de las escenas OpenGL. Implementa
    * el patrón de diseño Singleton. Está pensada para que las funciones
    * callback hagan llamadas a sus métodos.
    */
    class Renderer
    {
    private:
        static Renderer* instancia; ///< Puntero al único objeto

        Renderer();  // Constructor privado para el patrón Singleton
        ShaderProgram* shaderProgram; // Nuevo atributo para la gestión de shaders

        GLuint idVAO = 0; // Identificador del vertex array object
        GLuint idVBO = 0; // Identificador del vertex buffer object
        GLuint idIBO = 0; // Identificador del index buffer object

    public:
        virtual ~Renderer(); // Destructor

        /**
        * @brief Consulta del objeto único de la clase
        * @return Referencia a la instancia única de la clase
        */
        static Renderer& getInstancia();

        /**
        * @brief Método para refrescar la escena
        */
        void refrescar();

        /**
        * @brief Método para cambiar el tamaño del viewport
        * @param width Ancho del viewport
        * @param height Alto del viewport
        */
        void cambiarTamano(int width, int height);

        /**
        * @brief Método para cambiar el color de fondo
        * @param red Componente roja
        * @param green Componente verde
        * @param blue Componente azul
        * @param alpha Componente alfa (transparencia)
        */
        void cambiarColorFondo(float red, float green, float blue, float alpha);

        /**
        * @brief Método para crear el modelo 3D a renderizar
        */
        void creaModelo();

        /**
        * @brief Método para cargar un programa de shaders
        * @param shaderBase Nombre base de los archivos del shader
        */
        void loadShaderProgram(const std::string& shaderBase);
    };
}


#endif //PAGPR_RENDERER_H
