//
// Created by 34660 on 11/10/2024.
//

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "glad/glad.h"
#include <string>

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    void loadShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    void useProgram() const;
    GLuint getProgramID() const;

private:
    GLuint idSP;
    GLuint loadShader(const std::string& fileName, GLenum shaderType);
};

#endif // SHADERPROGRAM_H
