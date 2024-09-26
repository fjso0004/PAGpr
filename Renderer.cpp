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
        glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
        glEnable(GL_DEPTH_TEST);
    }

    /**
    * Destructor
    */
    Renderer::~Renderer ()
    { }

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

