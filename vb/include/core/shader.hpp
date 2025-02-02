#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/gl.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <spdlog/logger.h>

#include <string>

namespace vb {

struct Shader {
    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

    void use();

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string& name, const glm::vec3* const value) const;
    void setVec4(const std::string& name, const glm::vec4* const value) const;
    void setQuat(const std::string& name, const glm::quat* const value) const;
    void setMat4(const std::string& name, const glm::mat4* const value) const;

    unsigned int ID;
    std::shared_ptr<spdlog::logger> console;
};

}

#endif