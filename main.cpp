#include <iostream>
// IMPORTANTE: El include de GLAD debe estar siempre ANTES del de GLFW
#include "Renderer.h"
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_stdlib.h"
#include "ModeloOBJ.h"
#include "glm/gtc/type_ptr.hpp"

float red = 0.6f, green = 0.6f, blue = 0.6f, alpha = 1.0f;
std::string shaderBaseName = "../pag03";

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

void inicializarLuces(PAG::Renderer& renderer) {
    // Luz ambiente
    Luz luzAmbiente;
    luzAmbiente.tipo = TipoLuz::Ambiente;
    luzAmbiente.colorAmbiente = glm::vec3(0.2f, 0.2f, 0.2f);
    luzAmbiente.colorDifusa = glm::vec3(0.0f);
    luzAmbiente.colorEspecular = glm::vec3(0.0f);
    luzAmbiente.posicion = glm::vec3(0.0f);
    luzAmbiente.direccion = glm::vec3(0.0f);
    luzAmbiente.anguloApertura = 0.0f;
    luzAmbiente.atenuacion = 0.0f;
    renderer.addLuz(luzAmbiente);

    // Luz puntual
    Luz luzPuntual;
    luzPuntual.tipo = TipoLuz::Puntual;
    luzPuntual.colorAmbiente = glm::vec3(0.0f);
    luzPuntual.colorDifusa = glm::vec3(1.0f, 1.0f, 1.0f);
    luzPuntual.colorEspecular = glm::vec3(0.0f);
    luzPuntual.posicion = glm::vec3(5.0f, 5.0f, 5.0f);
    luzPuntual.direccion = glm::vec3(0.0f);
    luzPuntual.anguloApertura = 0.0f;
    luzPuntual.atenuacion = 1.0f;
    renderer.addLuz(luzPuntual);

    // Luz direccional
    Luz luzDireccional;
    luzDireccional.tipo = TipoLuz::Direccional;
    luzDireccional.colorAmbiente = glm::vec3(0.0f);
    luzDireccional.colorDifusa = glm::vec3(0.8f, 0.8f, 0.8f);
    luzDireccional.colorEspecular = glm::vec3(0.0f);
    luzDireccional.posicion = glm::vec3(0.0f); // No relevante para luz direccional
    luzDireccional.direccion = glm::vec3(-1.0f, -1.0f, -1.0f);
    luzDireccional.anguloApertura = 0.0f;
    luzDireccional.atenuacion = 0.0f;
    renderer.addLuz(luzDireccional);

    // Luz foco
    Luz luzFoco;
    luzFoco.tipo = TipoLuz::Foco;
    luzFoco.colorAmbiente = glm::vec3(0.0f);
    luzFoco.colorDifusa = glm::vec3(1.0f, 0.9f, 0.8f);
    luzFoco.colorEspecular = glm::vec3(0.0f);
    luzFoco.posicion = glm::vec3(1.0f, 1.0f, 1.0f);
    luzFoco.direccion = glm::vec3(0.0f, -1.0f, -1.0f);
    luzFoco.anguloApertura = 45.0f; // Ángulo del foco
    luzFoco.atenuacion = 1.0f;
    renderer.addLuz(luzFoco);
}

// Callback para manejar el estado del botón del ratón
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        botonIzquierdoPulsado = (action == GLFW_PRESS);
        if (botonIzquierdoPulsado) {
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
    float velocidad = 0.1f;
    float angulo = 5.0f;

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

int colorIndex = 0; // 0 = red, 1 = green, 2 = blue

// Esta función callback será llamada cada vez que se mueva la rueda del ratón sobre el área de dibujo OpenGL.
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    float colorChange = 0.05f * static_cast<float>(yoffset);
    std::cout << "Before Change - Red: " << red << ", Green: " << green << ", Blue: " << blue << " (Color index: " << colorIndex << ")" << std::endl;

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

    std::cout << "After Change - Red: " << red << ", Green: " << green << ", Blue: " << blue << " (Next Color index: " << colorIndex << ")" << std::endl;
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

    GLFWwindow* window = glfwCreateWindow(1024, 720, "PAG Introduction", nullptr, nullptr);

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
        //PAG::Renderer::getInstancia().cambiarModoVisualizacion(PAG::ModoVisualizacion::Solido);
        inicializarLuces(PAG::Renderer::getInstancia());
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -4;
    }

    ImVec4 color = ImVec4(red, green, blue, alpha);

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

        // ---- Ventana de control de color de fondo ----
        ImGui::Begin("Color de fondo");

        color.x = red;
        color.y = green;
        color.z = blue;

        // Selector de color
        ImGui::ColorPicker4("Actual", (float*)&color, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB);

        red = color.x;
        green = color.y;
        blue = color.z;
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
        static std::string rutaModelo = "../modelos/vaca.obj"; // Ruta para cargar un modelo
        static std::string rutaTextura = "../spot_texture.png";
        static int modeloSeleccionado = -1; // Índice del modelo seleccionado
        static float traslacion[3] = {0.0f, 0.0f, 0.0f};
        static float rotacion[3] = {0.0f, 0.0f, 0.0f};
        static float escala[3] = {1.0f, 1.0f, 1.0f};

        ImGui::Begin("Gestión de Modelos");

        ImGui::Text("Cargar nuevo modelo:");
        ImGui::InputText("Ruta del Modelo", &rutaModelo);
        if (ImGui::Button("Cargar Modelo")) {
            PAG::Renderer::getInstancia().cargarModelo(rutaModelo, rutaTextura);

            // Configurar el material para el modelo cargado
            auto& modelos = PAG::Renderer::getInstancia().getModels(); // Obtener referencia a los modelos
            if (!modelos.empty()) {
                Material material;
                material.colorAmbiente = glm::vec3(0.2f, 0.2f, 0.2f);  // Ambiente tenue
                material.colorDifuso = glm::vec3(0.8f, 0.5f, 0.3f);    // Color marrón
                material.colorEspecular = glm::vec3(1.0f, 1.0f, 1.0f); // Especular blanco
                modelos.back()->setMaterial(material); // Configura el material en el último modelo cargado
            }

            rutaModelo.clear();
        }


        ImGui::Separator();

        ImGui::Text("Modelos en la escena:");
        if (ImGui::BeginListBox("Modelos")) {
            const auto& modelos = PAG::Renderer::getInstancia().getModels(); // Obtener referencia a modelos
            for (size_t i = 0; i < modelos.size(); ++i) {
                const bool seleccionado = (modeloSeleccionado == static_cast<int>(i));
                if (ImGui::Selectable(("Modelo " + std::to_string(i+1)).c_str(), seleccionado)) {
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

        // ---- Ventana para la gestión de texturas ----
        ImGui::Begin("Gestión de Texturas");

        static std::string texturaRuta = "../texturas/spot_texture.png";
        static int modeloSeleccionadoParaTextura = -1; // Índice del modelo al que se asociará la textura

        ImGui::Text("Cargar Textura:");
        ImGui::InputText("Ruta de la Textura", &texturaRuta);

        if (ImGui::Button("Cargar Textura")) {
            try {
                if (modeloSeleccionadoParaTextura >= 0 && modeloSeleccionadoParaTextura < PAG::Renderer::getInstancia().getModels().size()) {
                    GLuint texturaID = PAG::Renderer::getInstancia().cargarTextura(texturaRuta);

                    auto& modelos = PAG::Renderer::getInstancia().getModels();
                    Material material = modelos[modeloSeleccionadoParaTextura]->getMaterial();
                    material.texturaID = texturaID; // Asignar la textura al material del modelo seleccionado
                    modelos[modeloSeleccionadoParaTextura]->setMaterial(material);

                    std::cout << "Textura cargada y asociada al modelo " << modeloSeleccionadoParaTextura << ": " << texturaRuta << std::endl;
                } else {
                    std::cerr << "Error: No hay un modelo seleccionado para asociar la textura." << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error al cargar la textura: " << e.what() << std::endl;
            }
        }

        ImGui::Separator();

        // Mostrar los modelos disponibles para asociar texturas
        ImGui::Text("Selecciona un modelo para asociar la textura:");
        if (ImGui::BeginListBox("Modelos")) {
            const auto& modelos = PAG::Renderer::getInstancia().getModels();
            for (size_t i = 0; i < modelos.size(); ++i) {
                const bool isSelected = (modeloSeleccionadoParaTextura == static_cast<int>(i));
                if (ImGui::Selectable(("Modelo " + std::to_string(i+1)).c_str(), isSelected)) {
                    modeloSeleccionadoParaTextura = static_cast<int>(i);
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndListBox();
        }

        if (modeloSeleccionadoParaTextura >= 0) {
            ImGui::Text("Modelo Seleccionado: %d", modeloSeleccionadoParaTextura);
        } else {
            ImGui::Text("No hay un modelo seleccionado.");
        }

        ImGui::End();

        ImGui::Begin("Material de Modelos");

        // Iterar sobre todos los modelos para mostrar y editar sus materiales
        const auto& modelos = PAG::Renderer::getInstancia().getModels();
        for (size_t i = 0; i < modelos.size(); ++i) {
            ImGui::Text("Modelo %zu:", i+1);

            static glm::vec3 colorDifuso = modelos[i]->getMaterial().colorDifuso;

            // Mostrar el selector de color para editar el color difuso
            ImGui::ColorEdit3(("Color Difuso##" + std::to_string(i)).c_str(), glm::value_ptr(colorDifuso));

            if (ImGui::Button(("Actualizar Material##" + std::to_string(i+1)).c_str())) {
                Material nuevoMaterial = modelos[i]->getMaterial();
                nuevoMaterial.colorAmbiente = glm::vec3(0.2f, 0.2f, 0.2f);  // Ejemplo
                nuevoMaterial.colorDifuso = colorDifuso; // Color editado en ImGui
                nuevoMaterial.colorEspecular = glm::vec3(1.0f, 1.0f, 1.0f); // Ejemplo
                modelos[i]->setMaterial(nuevoMaterial);

                // Debugging
                std::cout << "Material Actualizado: "
                          << "Ka: " << nuevoMaterial.colorAmbiente.x << ", " << nuevoMaterial.colorAmbiente.y << ", " << nuevoMaterial.colorAmbiente.z
                          << " | Kd: " << nuevoMaterial.colorDifuso.x << ", " << nuevoMaterial.colorDifuso.y << ", " << nuevoMaterial.colorDifuso.z
                          << " | Ks: " << nuevoMaterial.colorEspecular.x << ", " << nuevoMaterial.colorEspecular.y << ", " << nuevoMaterial.colorEspecular.z
                          << std::endl;
            }

            ImGui::Separator();
        }
        ImGui::End();


        ImGui::Begin("Modo de Visualización");
        if (ImGui::RadioButton("Alambre", static_cast<int>(PAG::Renderer::getInstancia().getModoVisualizacion()) == 0)) {
            PAG::Renderer::getInstancia().cambiarModoVisualizacion(PAG::ModoVisualizacion::Alambre);
        }
        if (ImGui::RadioButton("Sólido", static_cast<int>(PAG::Renderer::getInstancia().getModoVisualizacion()) == 1)) {
            PAG::Renderer::getInstancia().cambiarModoVisualizacion(PAG::ModoVisualizacion::Solido);
        }
        ImGui::End();


        ImGui::Begin("Luces");

        if (ImGui::Button("Recargar Luces")) {
            PAG::Renderer::getInstancia().clearLuces();
            inicializarLuces(PAG::Renderer::getInstancia());
        }

        static int luzSeleccionada = 0;
        if (ImGui::BeginListBox("Luces")) {
            for (size_t i = 0; i < PAG::Renderer::getInstancia().getLuces().size(); ++i) {
                const bool seleccionada = (luzSeleccionada == static_cast<int>(i));
                if (ImGui::Selectable(("Luz " + std::to_string(i+1)).c_str(), seleccionada)) {
                    luzSeleccionada = static_cast<int>(i);
                }
                if (seleccionada) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndListBox();
        }

        // Edita los parámetros de la luz seleccionada
        if (luzSeleccionada >= 0 && luzSeleccionada < PAG::Renderer::getInstancia().getLuces().size()) {
            Luz& luz = PAG::Renderer::getInstancia().getLuces()[luzSeleccionada];

            ImGui::Text("Editar Luz:");
            ImGui::ColorEdit3("Color Ambiente", glm::value_ptr(luz.colorAmbiente));
            ImGui::ColorEdit3("Color Difuso", glm::value_ptr(luz.colorDifusa));
            ImGui::ColorEdit3("Color Especular", glm::value_ptr(luz.colorEspecular));

            if (luz.tipo == TipoLuz::Puntual || luz.tipo == TipoLuz::Foco) {
                ImGui::InputFloat3("Posición", glm::value_ptr(luz.posicion));
            }

            if (luz.tipo == TipoLuz::Direccional || luz.tipo == TipoLuz::Foco) {
                ImGui::InputFloat3("Dirección", glm::value_ptr(luz.direccion));
                if (luz.tipo == TipoLuz::Foco) {
                    ImGui::SliderAngle("Apertura", &luz.anguloApertura, 0.0f, 90.0f);
                }
            }
        }

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
