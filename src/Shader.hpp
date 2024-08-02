#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include "TypeHelper.hpp"

class Shader {
private:
    unsigned int program;

public:
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    ~Shader();
    void Use() const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetVec3(const std::string& name, const Vector3f& value) const;
    void SetMat4(const std::string& name, const Matrix4x4f& value) const;
};

#endif