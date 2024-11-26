#include <iostream>
// IMPORTANTE: El include de GLAD debe estar siempre ANTES del de GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_stdlib.h"
#include "ModeloOBJ.h"

float red = 0.6f, green = 0.6f, blue = 0.6f, alpha = 1.0f; // Valores iniciales
std::string shaderBaseName = "../pag03"; // Nombre base de los shaders

// Variables globales para el control del ratón
bool botonIzquierdoPulsado = false;
double lastMouseX = 0.0, lastMouseY = 0.0;

// Enumeración para seleccionar el tipo de movimiento de cámara
enum class MovimientoCamara {
    Ninguno,
    Pan,
    Tilt,
    Dolly,
    Crane,
    Orbit,
    Zoom
};
MovimientoCamara movimientoActual = MovimientoCamara::Ninguno;  // Movimiento seleccionado

// Callback para manejar el estado del botón del ratón
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        botonIzquierdoPulsado = (action == GLFW_PRESS);
        if (botonIzquierdoPulsado) {
            // Obtiene la posición actual del ratón cuando se presiona
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        }
    }
}

// Callback para manejar el movimiento del ratón
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    Camara* camara = PAG::Renderer::getInstancia().getCamara();

    if (botonIzquierdoPulsado) {
        double deltaX = xpos - lastMouseX;
        double deltaY = ypos - lastMouseY;

        // Aplica el movimiento de cámara según el tipo seleccionado
        switch (movimientoActual) {
            case MovimientoCamara::Pan:
                camara->pan(deltaX * 0.1f);
                break;
            case MovimientoCamara::Tilt:
                camara->tilt(deltaY * 0.1f);
                break;
            case MovimientoCamara::Dolly:
                camara->dolly(deltaY * 0.05f);  // Dolly basado en el movimiento vertical
                break;
            case MovimientoCamara::Crane:
                camara->crane(deltaY * 0.05f);  // Crane basado en el movimiento vertical
                break;
            case MovimientoCamara::Orbit:
                camara->orbit(deltaY * 0.1f, deltaX * 0.1f);  // Orbit en ambos ejes
                break;
            case MovimientoCamara::Zoom:
                camara->zoom(deltaY * 0.1f);  // Zoom basado en el movimiento vertical
                break;
            default:
                break;
        }

        // Actualiza la última posición del ratón
        lastMouseX = xpos;
        lastMouseY = ypos;
    }
}


// - Esta función callback será llamada cuando GLFW produzca algún error
void error_callback(int errno, const char* desc) {
    std::string aux(desc);
    std::cout << "Error de GLFW número " << errno << ": " << aux << std::endl;
}

// - Esta función callback será llamada cada vez que el área de dibujo OpenGL deba ser redibujada.
void window_refresh_callback(GLFWwindow* window) {
    PAG::Renderer::getInstancia().refrescar();
    glfwSwapBuffers(window);
}

// - Esta función callback será llamada cada vez que se cambie el tamaño del área de dibujo OpenGL.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    PAG::Renderer::getInstancia().cambiarTamano(width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Camara* camara = PAG::Renderer::getInstancia().getCamara();
    float velocidad = 0.1f;  // Velocidad de traslación
    float angulo = 5.0f;     // Velocidad de rotación en grados

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (action == GLFW_PRESS) {
        switch (key) {
            // Movimiento Pan (horizontal)
            case GLFW_KEY_LEFT:
                camara->pan(-angulo);  // Pan a la izquierda
                break;
            case GLFW_KEY_RIGHT:
                camara->pan(angulo);  // Pan a la derecha
                break;

                // Movimiento Tilt (vertical)
            case GLFW_KEY_UP:
                camara->tilt(-angulo);  // Tilt hacia arriba
                break;
            case GLFW_KEY_DOWN:
                camara->tilt(angulo);  // Tilt hacia abajo
                break;

                // Movimiento Dolly (adelante/atrás)
            case GLFW_KEY_W:
                camara->dolly(velocidad);  // Mover hacia adelante
                break;
            case GLFW_KEY_S:
                camara->dolly(-velocidad);  // Mover hacia atrás
                break;

                // Movimiento Crane (arriba/abajo)
            case GLFW_KEY_Q:
                camara->crane(velocidad);  // Mover hacia arriba
                break;
            case GLFW_KEY_E:
                camara->crane(-velocidad);  // Mover hacia abajo
                break;

                // Movimiento Orbit (alrededor del punto de interés)
            case GLFW_KEY_A:
                camara->orbit(0.0f, angulo);  // Orbit izquierda
                break;
            case GLFW_KEY_D:
                camara->orbit(0.0f, -angulo);  // Orbit derecha
                break;
            case GLFW_KEY_Z:
                camara->orbit(angulo, 0.0f);  // Orbit arriba
                break;
            case GLFW_KEY_X:
                camara->orbit(-angulo, 0.0f);  // Orbit abajo
                break;

                // Zoom
            case GLFW_KEY_R:
                camara->zoom(-2.0f);  // Zoom In
                break;
            case GLFW_KEY_F:
                camara->zoom(2.0f);  // Zoom Out
                break;
        }
    }
}


// Variable global para controlar cuál componente de color está siendo modificado
int colorIndex = 0; // 0 = red, 1 = green, 2 = blue

// Esta función callback será llamada cada vez que se mueva la rueda del ratón sobre el área de dibujo OpenGL.
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    float colorChange = 0.05f * static_cast<float>(yoffset);

    // Debugging: Imprimimos los valores de color y el índice de color actual
    std::cout << "Before Change - Red: " << red << ", Green: " << green << ", Blue: " << blue << " (Color index: " << colorIndex << ")" << std::endl;

    // Dependiendo del índice de color, modificamos el componente correspondiente
    if (colorIndex == 0) {
        red = std::min(1.0f, std::max(0.0f, red + colorChange));

        // Si el rojo ha alcanzado su límite, pasamos a modificar el verde
        if (red == 1.0f || red == 0.0f) {
            colorIndex = 1; // Cambiamos al componente verde
        }
    }
    else if (colorIndex == 1) {
        green = std::min(1.0f, std::max(0.0f, green + colorChange));

        // Si el verde ha alcanzado su límite, pasamos a modificar el azul
        if (green == 1.0f || green == 0.0f) {
            colorIndex = 2; // Cambiamos al componente azul
        }
    }
    else if (colorIndex == 2) {
        blue = std::min(1.0f, std::max(0.0f, blue + colorChange));

        // Si el azul ha alcanzado su límite, volvemos a modificar el rojo
        if (blue == 1.0f || blue == 0.0f) {
            colorIndex = 0; // Volvemos al componente rojo
        }
    }

    // Debugging: Imprimimos los valores de color después del cambio
    std::cout << "After Change - Red: " << red << ", Green: " << green << ", Blue: " << blue << " (Next Color index: " << colorIndex << ")" << std::endl;

    // Cambiamos el color de fondo en el renderer
    PAG::Renderer::getInstancia().cambiarColorFondo(red, green, blue, alpha);

    // Forzamos un redibujado inmediato
    glfwSwapBuffers(window);
}


int main() {
    std::cout << "Starting Application PAG - Prueba 01" << std::endl;

    glfwSetErrorCallback((GLFWerrorfun)error_callback);

    if (glfwInit() != GLFW_TRUE) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // Activa antialiasing
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow* window = glfwCreateWindow(1024, 576, "PAG Introduction", nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "Failed to open GLFW window" << std::endl;
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "GLAD initialization failed" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -3;
    }

    std::cout << glGetString(GL_RENDERER) << std::endl
              << glGetString(GL_VENDOR) << std::endl
              << glGetString(GL_VERSION) << std::endl
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // Configuración de los callbacks de GLFW
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetWindowRefreshCallback(window, window_refresh_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Inicializamos ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Activa navegación por teclado

    ImGui::StyleColorsDark(); // Usa un esquema de colores oscuros

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // ---- Antes del ciclo principal ----
    try {
        // Carga los shaders iniciales
        PAG::Renderer::getInstancia().loadShaderProgram(shaderBaseName);
        PAG::Renderer::getInstancia().creaModelo();
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -4;
    }

    // Variables para controlar el color, inicializadas a valores globales
    ImVec4 color = ImVec4(red, green, blue, alpha);

    //ModeloOBJ* modelo = new ModeloOBJ("../vaca.obj");
    //if (modelo->cargarModelo()) {
    //    modelo->inicializarBuffers();
    //}

    // Ciclo de eventos de la aplicación
    while (!glfwWindowShouldClose(window)) {
        // Poll de eventos
        glfwPollEvents();

        // Iniciar un nuevo frame de ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ---- Ventana para cambiar shaders ----
        ImGui::Begin("Shaders");
        ImGui::Text("Selecciona el nombre base de los shaders:");
        ImGui::InputText("Shader Base", &shaderBaseName, ImGuiInputTextFlags_AutoSelectAll);

        if (ImGui::Button("Cargar Shaders")) {
            try {
                PAG::Renderer::getInstancia().loadShaderProgram(shaderBaseName);
            }
            catch (const std::runtime_error& e) {
                std::cerr << "Error al cargar shaders: " << e.what() << std::endl;
            }
        }

        ImGui::End();

        // Ventana de control de color de fondo
        ImGui::Begin("Color de fondo");

        // Sincronizamos el picker de ImGui con los valores globales
        color.x = red;
        color.y = green;
        color.z = blue;

        // Selector de color
        ImGui::ColorPicker4("Actual", (float*)&color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB);

        // Actualizamos los valores globales con los seleccionados en ImGui
        red = color.x;
        green = color.y;
        blue = color.z;

        // Cambiamos el color de fondo con los valores actualizados
        PAG::Renderer::getInstancia().cambiarColorFondo(red, green, blue, alpha);

        ImGui::End();

        // ---- Ventana para seleccionar el tipo de movimiento de cámara ----
        ImGui::Begin("Control de Cámara");

        ImGui::Text("Selecciona el tipo de movimiento:");
        if (ImGui::RadioButton("Pan", movimientoActual == MovimientoCamara::Pan)) {
            movimientoActual = MovimientoCamara::Pan;
        }
        if (ImGui::RadioButton("Tilt", movimientoActual == MovimientoCamara::Tilt)) {
            movimientoActual = MovimientoCamara::Tilt;
        }
        if (ImGui::RadioButton("Dolly", movimientoActual == MovimientoCamara::Dolly)) {
            movimientoActual = MovimientoCamara::Dolly;
        }
        if (ImGui::RadioButton("Crane", movimientoActual == MovimientoCamara::Crane)) {
            movimientoActual = MovimientoCamara::Crane;
        }
        if (ImGui::RadioButton("Orbit", movimientoActual == MovimientoCamara::Orbit)) {
            movimientoActual = MovimientoCamara::Orbit;
        }
        if (ImGui::RadioButton("Zoom", movimientoActual == MovimientoCamara::Zoom)) {
            movimientoActual = MovimientoCamara::Zoom;
        }

        ImGui::End();

        // ---- Ventana para cargar y gestionar modelos ----
        static std::string rutaModelo; // Ruta para cargar un modelo
        static int modeloSeleccionado = -1; // Índice del modelo seleccionado
        static float traslacion[3] = {0.0f, 0.0f, 0.0f};
        static float rotacion[3] = {0.0f, 0.0f, 0.0f};
        static float escala[3] = {1.0f, 1.0f, 1.0f};

        ImGui::Begin("Gestión de Modelos");

// Entrada de ruta y botón para cargar modelos
        ImGui::Text("Cargar nuevo modelo:");
        ImGui::InputText("Ruta del Modelo", &rutaModelo);
        if (ImGui::Button("Cargar Modelo")) {
            PAG::Renderer::getInstancia().cargarModelo(rutaModelo);
            rutaModelo.clear(); // Limpiar el campo después de cargar
        }

        ImGui::Separator();

// Lista de modelos cargados
        ImGui::Text("Modelos en la escena:");
        if (ImGui::BeginListBox("Modelos")) {
            const auto& modelos = PAG::Renderer::getInstancia().getModels(); // Obtener referencia a modelos
            for (size_t i = 0; i < modelos.size(); ++i) {
                const bool seleccionado = (modeloSeleccionado == static_cast<int>(i));
                if (ImGui::Selectable(("Modelo " + std::to_string(i)).c_str(), seleccionado)) {
                    modeloSeleccionado = static_cast<int>(i);
                }
                if (seleccionado) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndListBox();
        }

// Si hay un modelo seleccionado, mostrar controles
        if (modeloSeleccionado >= 0) {
            if (ImGui::Button("Eliminar Modelo")) {
                PAG::Renderer::getInstancia().eliminarModelo(modeloSeleccionado);
                modeloSeleccionado = -1; // Reiniciar selección
            }

            ImGui::Separator();
            ImGui::Text("Transformaciones:");

            ImGui::InputFloat3("Traslación", traslacion);
            ImGui::InputFloat3("Rotación (grados)", rotacion);
            ImGui::InputFloat3("Escala", escala);

            // Construir la transformación y actualizar el modelo
            glm::mat4 transformacion = glm::mat4(1.0f);
            transformacion = glm::translate(transformacion, glm::vec3(traslacion[0], traslacion[1], traslacion[2]));
            transformacion = glm::rotate(transformacion, glm::radians(rotacion[0]), glm::vec3(1.0f, 0.0f, 0.0f));
            transformacion = glm::rotate(transformacion, glm::radians(rotacion[1]), glm::vec3(0.0f, 1.0f, 0.0f));
            transformacion = glm::rotate(transformacion, glm::radians(rotacion[2]), glm::vec3(0.0f, 0.0f, 1.0f));
            transformacion = glm::scale(transformacion, glm::vec3(escala[0], escala[1], escala[2]));

            PAG::Renderer::getInstancia().actualizarTransformacion(modeloSeleccionado, transformacion);
        }

        ImGui::End();

        // Refrescamos la escena de OpenGL
        PAG::Renderer::getInstancia().refrescar();

        // Renderizamos el frame de ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        //modelo->renderizar();

        // Intercambiamos los buffers
        glfwSwapBuffers(window);
    }

    //delete modelo;

    // Limpieza de ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Cierra la ventana y libera los recursos
    glfwDestroyWindow(window);
    window = nullptr;
    glfwTerminate();

    return 0;
}
