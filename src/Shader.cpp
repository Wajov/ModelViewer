#include "Shader.h"

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath) {
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexShaderSource;
    std::ifstream vertexShaderStream(vertexShaderPath, std::ios::in);
    if (vertexShaderStream.is_open()) {
        std::stringstream stringStream;
        stringStream << vertexShaderStream.rdbuf();
        vertexShaderSource = stringStream.str();
        vertexShaderStream.close();
    } else {
        std::cerr << "Failed to open vertex shader file" << std::endl;
        this->program = 0;
        return;
    }
    const char *vertexShaderPointer = vertexShaderSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderPointer, NULL);
    glCompileShader(vertexShader);
    int vertexShaderSuccess;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderSuccess);
    if (!vertexShaderSuccess) {
        char info[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, info);
        std::cerr << "Failed to compile vertex shader source:" << std::endl << info << std::endl;
        this->program = 0;
        return;
    }

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentShaderSource;
    std::ifstream fragmentShaderStream(fragmentShaderPath, std::ios::in);
    if (fragmentShaderStream.is_open()) {
        std::stringstream stringStream;
        stringStream << fragmentShaderStream.rdbuf();
        fragmentShaderSource = stringStream.str();
        fragmentShaderStream.close();
    } else {
        std::cerr << "Failed to open fragment shader file" << std::endl;
        this->program = 0;
        return;
    }
    const char *fragmentShaderPointer = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderPointer, NULL);
    glCompileShader(fragmentShader);
    int fragmentShaderSucess;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderSucess);
    if (!fragmentShaderSucess) {
        char info[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, info);
        std::cerr << "Failed to compile fragment shader source:" << std::endl << info << std::endl;
        this->program = 0;
        return;
    }

    int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    int shaderProgramSucess;
    glGetProgramiv(program, GL_LINK_STATUS, &shaderProgramSucess);
    if (!shaderProgramSucess) {
        char info[512];
        glGetProgramInfoLog(program, 512, NULL, info);
        std::cerr << "Failed to link shader program:" << std::endl << info << std::endl;
        this->program = 0;
        return;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    this->program = program;
}

void Shader::use() {
    glUseProgram(program);
}

void Shader::setInt(const char *name, int value) {
    glUniform1i(glGetUniformLocation(program, name), value);
}

void Shader::setFloat(const char *name, float value) {
    glUniform1f(glGetUniformLocation(program, name), value);
}

void Shader::setVec2(const char *name, glm::vec2 value) {
    glUniform2fv(glGetUniformLocation(program, name), 1, &value[0]);
}

void Shader::setVec3(const char *name, glm::vec3 value) {
    glUniform3fv(glGetUniformLocation(program, name), 1, &value[0]);
}

void Shader::setVec4(const char *name, glm::vec4 value) {
    glUniform4fv(glGetUniformLocation(program, name), 1, &value[0]);
}

void Shader::setMat4(const char *name, glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, &value[0][0]);
}