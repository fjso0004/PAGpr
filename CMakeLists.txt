cmake_minimum_required(VERSION 3.28.1)
project(PAGpr)

set(CMAKE_CXX_STANDARD 20)

file(GLOB MY_FILES *.cpp)
file(GLOB GLAD_FILES GLAD/src/*.c)
file(GLOB IMGUI_FILES imgui/*.cpp imgui/imgui_stdlib.cpp)

set(BUILD_SHARED_LIBS ON CACHE INTERNAL "" FORCE)

add_executable(PAGpr main.cpp ${MY_FILES} ${GLAD_FILES} ${IMGUI_FILES}
        Renderer.cpp
        Renderer.h
        ShaderProgram.cpp
        ShaderProgram.h
        Camera.cpp
        Camera.h
        Camera.cpp
        ModeloOBJ.cpp
        ModeloOBJ.h
        Luz.cpp
        Luz.h
)

target_include_directories(${PROJECT_NAME} PUBLIC GLAD/include
        imgui/include
        GLM
)

find_package(glfw3 REQUIRED)
find_package(opengl_system REQUIRED)
find_package(tinyobjloader REQUIRED)
target_link_libraries(${PROJECT_NAME} opengl::opengl glfw tinyobjloader::tinyobjloader)