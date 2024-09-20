cmake_minimum_required(VERSION 3.29)
project(PAGpr)

set(CMAKE_CXX_STANDARD 20)
# Define variables agrupando nombres de archivo
file( GLOB MY_FILES *.cpp )
file( GLOB GLAD_FILES GLAD/src/*.c )
# Indica qué archivos hay que compilar para generar el ejecutable

add_executable(PAGpr main.cpp ${MY_FILES} ${GLAD_FILES}
        Renderer.cpp
        Renderer.h)

# Indica directorios (adicionales a los por defecto) para buscar includes
target_include_directories( ${PROJECT_NAME} PUBLIC GLAD/include )
# Indica bibliotecas a enlazar en el ejecutable
find_package(glfw3)
find_package(opengl_system)

target_link_libraries( ${PROJECT_NAME} opengl::opengl glfw )

