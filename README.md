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

### Respuesta a la pregunta: Si redimensionas la ventana de la aplicación, verás que el triángulo
No permanece igual, sino que se deforma al mismo tiempo que la ventana. ¿A qué crees que se
debe este comportamiento?

Creo que este comportamiento se debe a que no se refresca la aplicación de openGL para mantener la relación de aspecto, simplemente se redmiensiona la imagen y se deforma.

## Práctica 4

He reordenado el código, en el uml queda reflejado los cambios de una forma clara y concisa, además, he conseguido que vuelva a funcionar el scroll callback con mi rueda del ratón para que modifique el color de fondo de la escena.

Enlace a mi [UML](UML.puml)

## Práctica 5

Para la implementación de las diversas cámaras, he creado una nueva clase, en la que he adjuntado todo lo relacionado con las mismas. En el main, el bucle de ciclo de eventos de la aplicación ahora ha sido actualizado junto con los callbacks para usar las nuevas cámaras.
En cuanto a la funcionalidad, he descargado y implementado la carpeta de GLM en el archivo raíz de el proyecto y añadido manualmente a cmake.
Ahora, al ejecutar, aparece una menú desplegable creado con imgui en el que seleccionamos el tipo de cámara que queremos utilizar, y usamos el ratón para desplazar la vista.

## Práctica 6

El uso de assimp fue imposible por errores con el puig.xml, assimp, conan y un error de librerías con basic_string, por lo que se opto por utilizar tinyobjloader.
Se concentra todo en la clase ModeloOBJ, que contiene todo lo necesario para utilizar un archivo .obj y renderizarlo en la escena, a través de renderer.
Se han creado nuevos métodos en la clase Renderer para utilizar esta clase, y se han añadido atributos para las transformaciones en él.

#### Como usar

Para usar la aplicación, se pone el directorio raíz que es en el que se encuentra el único objeto que hemos dejado en el proyecto para no cargarlo demasiado (../vaca.obj), se puede poner cuantas veces quiera, las flechas lo mueven en las distintas posiciones a las que apuntan las mismas.
Al crear el modelo, pincha sobre este en la interfaz de ImGui para interactuar con ella y hacer las transformaciones que se requiera.

## Práctica 7

