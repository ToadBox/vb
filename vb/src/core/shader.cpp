#include "core/shader.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "spdlog/spdlog.h"

#include <iostream>
#include <fstream>
#include <sstream>

vb::Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath) {
    console = spdlog::get("console");
    if (console == nullptr) {
        printf("WARN: Could not attach to main console");
    }

    // retrieve source code from file path
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // read the files
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vShaderFile.open(vertexShaderPath);
        fShaderFile.open(fragmentShaderPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (const std::ifstream::failure& e) {
        if (console.get() != nullptr) {
            console->critical("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ @ {} or {}", vertexShaderPath, fragmentShaderPath);
        } else
            printf("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n");
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // attempt to compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, sizeof(infoLog), NULL, infoLog);
        if (console.get() != nullptr) {
            console->critical("ERROR::SHADER::VERTEX::COMPILATION_FAILED {} @ ", infoLog, vertexShaderPath);
        } else
            printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, sizeof(infoLog), NULL, infoLog);
        if (console.get() != nullptr) {
            console->critical("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED {} @ {}", infoLog, fragmentShaderPath);
        } else
            printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
    }

    // attempt to link shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, sizeof(infoLog), NULL, infoLog);
        if (console.get() != nullptr) {
            console->critical("ERROR::SHADER::PROGRAM::LINKING_FAILED {} @ {} with {}", infoLog, vertexShaderPath, fragmentShaderPath);
        } else
            printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void vb::Shader::use() {
    glUseProgram(ID);
}

void vb::Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}

void vb::Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void vb::Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void vb::Shader::setVec3(const std::string& name, const glm::vec3* const value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(*value));
}

void vb::Shader::setVec4(const std::string& name, const glm::vec4 *const value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(*value));
}

void vb::Shader::setQuat(const std::string& name, const glm::quat *const value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(*value));
}

void vb::Shader::setMat4(const std::string &name, const glm::mat4 *const value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(*value));
}