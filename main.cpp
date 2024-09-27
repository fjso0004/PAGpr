#include <iostream>
// IMPORTANTE: El include de GLAD debe estar siempre ANTES de el de GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


float red = 0.6f, green = 0.6f, blue = 0.6f, alpha = 1.0f; // Valores iniciales

// - Esta función callback será llamada cuando GLFW produzca algún error
void error_callback ( int errno, const char* desc )
{ std::string aux (desc);
    std::cout << "Error de GLFW número " << errno << ": " << aux << std::endl;
}

// - Esta función callback será llamada cada vez que el área de dibujo OpenGL deba ser redibujada.
void window_refresh_callback ( GLFWwindow *window )
{
    PAG::Renderer::getInstancia().refrescar();
// - GLFW usa un doble buffer para que no haya parpadeo. Esta orden
// intercambia el buffer back (que se ha estado dibujando) por el
// que se mostraba hasta ahora front. Debe ser la última orden de
// este callback
    glfwSwapBuffers ( window );
    std::cout << "Refresh callback called" << std::endl;
}

// - Esta función callback será llamada cada vez que se cambie el tamaño del área de dibujo OpenGL.
void framebuffer_size_callback ( GLFWwindow *window, int width, int height )
{
    PAG::Renderer::getInstancia().cambiarTamano(width, height);
    std::cout << "Resize callback called" << std::endl;
}

// - Esta función callback será llamada cada vez que se pulse una tecla dirigida al área de dibujo OpenGL.
void key_callback ( GLFWwindow *window, int key, int scancode, int action, int mods )
{ if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
    { glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    std::cout << "Key callback called" << std::endl;
}

// - Esta función callback será llamada cada vez que se pulse algún botón del ratón sobre el área de dibujo OpenGL.
void mouse_button_callback ( GLFWwindow *window, int button, int action, int mods )
{ if ( action == GLFW_PRESS )
    { std::cout << "Pulsado el boton: " << button << std::endl;
    }
    else if ( action == GLFW_RELEASE )
    { std::cout << "Soltado el boton: " << button << std::endl;
    }
}

// - Esta función callback será llamada cada vez que se mueva la rueda del ratón sobre el área de dibujo OpenGL.
void scroll_callback ( GLFWwindow *window, double xoffset, double yoffset )
{ // Ajusta el color según el desplazamiento vertical (yoffset)
    // Ajusta el color según el desplazamiento vertical (yoffset)
    float colorChange = 0.05f * static_cast<float>(yoffset);

    red = std::min(1.0f, std::max(0.0f, red + colorChange));
    green = std::min(1.0f, std::max(0.0f, green + colorChange));
    blue = std::min(1.0f, std::max(0.0f, blue + colorChange));

    // Llamamos a Renderer para cambiar el color
    PAG::Renderer::getInstancia().cambiarColorFondo(red, green, blue, alpha);

    // Intercambia los buffers para reflejar los cambios en pantalla
    glfwSwapBuffers(window);
}

int main() {
    std::cout << "Starting Application PAG - Prueba 01" << std::endl;

//Este callback hay que registrarlo ANTES de llamar a glfwInit
    glfwSetErrorCallback ( (GLFWerrorfun) error_callback );

//Inicializa GLFW. Es un proceso que sólo debe realizarse una vez en la aplicación
    if ( glfwInit () != GLFW_TRUE )
    { std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

//Definimos las características que queremos que tenga el contexto gráfico
//OpenGL de la ventana que vamos a crear. Por ejemplo, el número de muestras o el modo Core Profile.
    glfwWindowHint ( GLFW_SAMPLES, 4 ); // - Activa antialiasing con 4 muestras.
    glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE ); // - Esta y las 2
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 4 ); // siguientes activan un contexto
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 1 ); // OpenGL Core Profile 4.1.

//Definimos el puntero para guardar la dirección de la ventana de la aplicación y la creamos
    GLFWwindow *window;

//Tamaño, título de la ventana, en ventana y no en pantalla completa, sin compartir recursos con otras ventanas.
    window = glfwCreateWindow ( 1024, 576, "PAG Introduction", nullptr, nullptr );

//Comprobamos si la creación de la ventana ha tenido éxito.
    if ( window == nullptr )
    { std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate (); // - Liberamos los recursos que ocupaba GLFW.
        return -2;
    }

//Hace que el contexto OpenGL asociado a la ventana que acabamos de crear pase a
//ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
    glfwMakeContextCurrent ( window );

//Ahora inicializamos GLAD.
    if ( !gladLoadGLLoader ( (GLADloadproc) glfwGetProcAddress ) )
    { std::cout << "GLAD initialization failed" << std::endl;
        glfwDestroyWindow ( window ); // - Liberamos los recursos que ocupaba GLFW.
        window = nullptr;
        glfwTerminate ();
        return -3;
    }

//Interrogamos a OpenGL para que nos informe de las propiedades del contexto 3D construido.
    std::cout << glGetString ( GL_RENDERER ) << std::endl
              << glGetString ( GL_VENDOR ) << std::endl
            << glGetString ( GL_VERSION ) << std::endl
            << glGetString ( GL_SHADING_LANGUAGE_VERSION ) << std::endl;

    //Registramos los callbacks que responderán a los eventos principales
    glfwSetWindowRefreshCallback ( window, window_refresh_callback );
    glfwSetFramebufferSizeCallback ( window, framebuffer_size_callback );
    glfwSetKeyCallback ( window, key_callback );
    glfwSetMouseButtonCallback ( window, mouse_button_callback );
    glfwSetScrollCallback ( window, scroll_callback );

//Establecemos un gris medio como color con el que se borrará el frame buffer.
// No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
    PAG::Renderer::getInstancia().cambiarColorFondo(red, green, blue, alpha);  // Establece el color de fondo inical


//Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
// No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
    glEnable ( GL_DEPTH_TEST );

    // ---- Inicialización de Dear ImGui ----
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Activa navegación por teclado

    ImGui::StyleColorsDark();  // Usa un esquema de colores oscuros

    // Inicializa los backends de GLFW y OpenGL para Dear ImGui
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    // -------------------------------------

//Ciclo de eventos de la aplicación. La condición de parada es que la ventana principal
//deba cerrarse, por ejemplo, si el usuario pulsa el botón de cerrar la ventana (la X).
    while ( !glfwWindowShouldClose ( window ) )
    {
//Obtiene y organiza los eventos pendientes, tales como pulsaciones
//de teclas o de ratón, etc. Siempre al final de cada iteración del
//ciclo de eventos y después de glfwSwapBuffers ( window );
        glfwPollEvents ();

        // ---- Nuevo frame de ImGui ----
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // --------------------------------

        // ---- Ventana de control de color (Fondo) ----
        ImGui::Begin("Fondo");
        static ImVec4 color = ImVec4(red, green, blue, alpha);  // Color seleccionado

        // Control de color avanzado con rueda y triángulo
        ImGui::Text("Selecciona un color:");

        // Usamos ColorPicker4 con ImGuiColorEditFlags para mostrar el círculo con triángulo
        ImGui::ColorPicker4("Actual", (float*)&color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB);

        // Mostrar valores RGB y hexadecimal
        ImGui::Text("R: %d G: %d B: %d",
                    (int)(color.x * 255),
                    (int)(color.y * 255),
                    (int)(color.z * 255));
        ImGui::Text("#%02X%02X%02X",
                    (int)(color.x * 255),
                    (int)(color.y * 255),
                    (int)(color.z * 255));

        // Actualizamos el color de fondo de OpenGL con el nuevo color seleccionado
        red = color.x;
        green = color.y;
        blue = color.z;
        PAG::Renderer::getInstancia().cambiarColorFondo(red, green, blue, alpha);
        ImGui::End();
        // --------------------------------------------


        // Refresca la ventana de OpenGL
        PAG::Renderer::getInstancia().refrescar();

        // ---- Renderiza el frame de ImGui ----
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // -------------------------------------

        // Intercambia los buffers
        glfwSwapBuffers(window);
    }

// Establecemos un gris medio como color con el que se borrará el frame buffer.
//No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
    // Control de color de ImGui
    ImGui::ColorEdit3("Color de fondo", (float*)&red);
    PAG::Renderer::getInstancia().cambiarColorFondo(red, green, blue, alpha);  // Cambiar el color de fondo

//Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
//No tiene por qué ejecutarse en cada paso por el ciclo de eventos.
    glEnable ( GL_DEPTH_TEST );

//Ciclo de eventos de la aplicación. La condición de parada es que la
//ventana principal deba cerrarse. Por ejemplo, si el usuario pulsa el
//botón de cerrar la ventana (la X).
    while ( !glfwWindowShouldClose ( window ) )
    {
//Borra los buffers (color y profundidad)
        PAG::Renderer::getInstancia().refrescar();
//GLFW usa un doble buffer para que no haya parpadeo. Esta orden
//intercambia el buffer back (en el que se ha estado dibujando) por el que se mostraba hasta ahora (front).
        glfwSwapBuffers ( window );
//Obtiene y organiza los eventos pendientes, tales como pulsaciones de
//teclas o de ratón, etc. Siempre al final de cada iteración del ciclo
//de eventos y después de glfwSwapBuffers(window);
        glfwPollEvents ();
    }

    // ---- Limpieza de ImGui ----
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // --------------------------

//Una vez terminado el ciclo de eventos, liberar recursos, etc.
    std::cout << "Finishing application pag prueba" << std::endl;
//Cerramos y destruimos la ventana de la aplicación.
    glfwDestroyWindow ( window );
    window = nullptr;
//Liberamos los recursos que ocupaba GLFW.
    glfwTerminate ();
}
