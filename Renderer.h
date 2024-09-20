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
    };
}


#endif //PAGPR_RENDERER_H
