#include <iostream>
// IMPORTANTE: El include de GLAD debe estar siempre ANTES del de GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_stdlib.h"

float red = 0.6f, green = 0.6f, blue = 0.6f, alpha = 1.0f; // Valores iniciales
std::string shaderBaseName = "../pag03"; // Nombre base de los shaders

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

// - Esta función callback será llamada cada vez que se pulse una tecla dirigida al área de dibujo OpenGL.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
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

    // Ciclo de eventos de la aplicación
    while (!glfwWindowShouldClose(window)) {
        // Poll de eventos
        glfwPollEvents();

        // Nuevo frame de ImGui
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

        // Refrescamos la escena de OpenGL
        PAG::Renderer::getInstancia().refrescar();

        // Renderizamos el frame de ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Intercambiamos los buffers
        glfwSwapBuffers(window);
    }

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
