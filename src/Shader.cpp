#include "Shader.h"

Shader::Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) {
    bool vertexShaderSuccess, fragmentShaderSuccess;
    int vertexShader = processShader(vertexShaderPath, GL_VERTEX_SHADER, vertexShaderSuccess);
    int fragmentShader = processShader(fragmentShaderPath, GL_FRAGMENT_SHADER, fragmentShaderSuccess);
    if (!vertexShaderSuccess || !fragmentShaderSuccess) {
        this->program = 0;
        return;
    }

    int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char info[512];
        glGetProgramInfoLog(program, 512, nullptr, info);
        std::cerr << "Failed to link shader program:" << std::endl << info << std::endl;
        this->program = 0;
        return;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    this->program = program;
}

Shader::~Shader() {}

int Shader::processShader(const std::string &path, unsigned int type, bool &success) {
    std::ifstream fileStream(path);
    if (!(success = fileStream.is_open())) {
        std::cerr << "Failed to open shader file " << path << std::endl;
        return 0;
    }

    std::stringstream stringStream;
    stringStream << fileStream.rdbuf();
    fileStream.close();
    std::string source = stringStream.str();

    int shader = glCreateShader(type);
    const char *pointer = source.c_str();
    glShaderSource(shader, 1, &pointer, nullptr);
    glCompileShader(shader);
    int compileSuccess;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
    if (!(success = compileSuccess)) {
        char info[512];
        glGetShaderInfoLog(shader, 512, nullptr, info);
        std::cerr << "Failed to compile shader file " << path << ":" << std::endl << info << std::endl;
        return 0;
    }

    return shader;
}

void Shader::use() {
    glUseProgram(program);
}

void Shader::setInt(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, glm::vec2 &value) {
    glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, glm::vec3 &value) {
    glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, glm::vec4 &value) {
    glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::setMat4(const std::string &name, glm::mat4 &value) {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &value[0][0]);
}