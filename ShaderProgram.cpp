//
// Created by 34660 on 11/10/2024.
//

#include "ShaderProgram.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

ShaderProgram::ShaderProgram() : idSP(0) {}

ShaderProgram::~ShaderProgram() {
    if (idSP != 0) {
        glDeleteProgram(idSP);
    }
}

void ShaderProgram::loadShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    GLuint vertexShader = loadShader(vertexShaderPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = loadShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

    idSP = glCreateProgram();
    glAttachShader(idSP, vertexShader);
    glAttachShader(idSP, fragmentShader);
    glLinkProgram(idSP);

    GLint linkStatus;
    glGetProgramiv(idSP, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        GLint logLength;
        glGetProgramiv(idSP, GL_INFO_LOG_LENGTH, &logLength);
        std::string logMessage(logLength, ' ');
        glGetProgramInfoLog(idSP, logLength, nullptr, &logMessage[0]);
        throw std::runtime_error("Error linking shader program: " + logMessage);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void ShaderProgram::useProgram() const {
    glUseProgram(idSP);
}

GLuint ShaderProgram::getProgramID() const {
    return idSP;
}

GLuint ShaderProgram::loadShader(const std::string& fileName, GLenum shaderType) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader file: " + fileName);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();
    const char* codeCStr = code.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &codeCStr, nullptr);
    glCompileShader(shader);

    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE) {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::string logMessage(logLength, ' ');
        glGetShaderInfoLog(shader, logLength, nullptr, &logMessage[0]);
        throw std::runtime_error("Error compiling shader: " + logMessage);
    }

    return shader;
}
