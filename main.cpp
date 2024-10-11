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

// - Esta función callback será llamada cada vez que se mueva la rueda del ratón sobre el área de dibujo OpenGL.
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    float colorChange = 0.05f * static_cast<float>(yoffset);

    red = std::min(1.0f, std::max(0.0f, red + colorChange));
    green = std::min(1.0f, std::max(0.0f, green + colorChange));
    blue = std::min(1.0f, std::max(0.0f, blue + colorChange));

    PAG::Renderer::getInstancia().cambiarColorFondo(red, green, blue, alpha);
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

    // ---- Ciclo de eventos de la aplicación ----
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // ---- Nuevo frame de ImGui ----
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

        // ---- Ventana de control de color ----
        ImGui::Begin("Color de fondo");
        static ImVec4 color = ImVec4(red, green, blue, alpha);

        ImGui::Text("Selecciona un color:");
        ImGui::ColorPicker4("Actual", (float*)&color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB);

        // Actualizamos el color de fondo con el nuevo color seleccionado
        red = color.x;
        green = color.y;
        blue = color.z;
        PAG::Renderer::getInstancia().cambiarColorFondo(red, green, blue, alpha);

        ImGui::End();

        // Refresca la ventana de OpenGL
        PAG::Renderer::getInstancia().refrescar();

        // ---- Renderiza el frame de ImGui ----
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Intercambia los buffers
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
