#ifndef SHADER_H
#define SHADER_H

#include <bits/stdc++.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
private:
    unsigned int program;

public:
    Shader(const char *vertexShaderPath, const char *fragmentShaderPath);
    void use();
    void setInt(const char *name, int value);
    void setFloat(const char *name, float value);
    void setVec2(const char *name, glm::vec2 value);
    void setVec3(const char *name, glm::vec3 value);
    void setVec4(const char *name, glm::vec4 value);
    void setMat4(const char *name, glm::mat4 value);
};

#endif