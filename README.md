# PAGpr

## Práctica 1

Para implementar el cmabio de color con la rotación de la rueda del ratón, implementamos unas variables globales rgb para usarlas dentro de la función void scroll_callbacks, y dentro del main.
A continuación, hemos modificado la función para que usando la variable yoffset, se utilice para incrementar o reducir los valores, siempre dentro del rango [0,1] (para ello usamos std::min y std::max).
Después, forzamos la ventana a redibujarse con glclear y glfwSwapBuffers, para que el cambio se refleje de inmediato

## Práctica 2

Hemos implementado un sistema de renderizado utilizando OpenGL junto con Dear ImGui para crear una interfaz gráfica de usuario interactiva. Inicialmente, configuramos el entorno OpenGL y GLFW para manejar ventanas y eventos, integrando la funcionalidad de Dear ImGui para añadir ventanas flotantes personalizables. Gestionamos eventos de entrada, como clics de ratón, y mostrar información relevante en tiempo real. Luego, implementamos un control de color avanzado utilizando el widget ImGui::ColorPicker4, lo que permitie seleccionar un color de fondo con un selector de tonos en forma de rueda y triángulo, como se ve en la interfaz gráfica. 
Para lograrlo, actualizamos el ciclo principal de eventos, donde añadimos la renderización de la GUI y vinculamos el color seleccionado en el picker de ImGui con el color de fondo del contexto OpenGL. Finalmente, encapsulamos la lógica de renderizado dentro de la clase PAG::Renderer para organizar el código, sustituyendo las llamadas directas a OpenGL por funciones controladas a través de la clase.

## Práctica 3

En esta aplicación, hemos implementado la carga de shaders desde archivos externos mediante la función loadShaderProgram, que toma como parámetro un archivo "pag03" y luego carga los archivos de shaders con loadShader(archivo + "-vs.glsl", GL_VERTEX_SHADER) y loadShader(archivo + "-fs.glsl", GL_FRAGMENT_SHADER). 
Estos archivos GLSL se encuentran en la carpeta raíz y se cargan usando rutas relativas. Además, se ha añadido manejo de errores en la compilación y enlace de shaders, lanzando excepciones con throw std::runtime_error(mensaje) si ocurre un fallo, garantizando así que se detecten y manejen adecuadamente los errores.