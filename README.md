# PAGpr

## Práctica 1

Para implementar el cmabio de color con la rotación de la rueda del ratón, implementamos unas variables globales rgb para usarlas dentro de la función void scroll_callbacks, y dentro del main.
A continuación, hemos modificado la función para que usando la variable yoffset, se utilice para incrementar o reducir los valores, siempre dentro del rango [0,1] (para ello usamos std::min y std::max).
Después, forzamos la ventana a redibujarse con glclear y glfwSwapBuffers, para que el cambio se refleje de inmediato

## Práctica 2


