#include "Shader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "vendor/glm/gtc/type_ptr.hpp"

Shader::Shader(const std::string& filepath) : RendererID_(0), FilePath_(filepath) {
    ShaderProgramSource source = ParseShader(filepath);
    RendererID_ = CreateShader(source.VertexSource, source.FragmentSource);
}

void Shader::Bind() const {
    GLCall(glUseProgram(RendererID_))
}

void Shader::Unbind() const {
    GLCall(glUseProgram(0))
}

void Shader::SetUniform1i(const std::string& name, int value) {
    GLCall(glUniform1i(GetUniformLocation(name), value))
}

void Shader::SetUniform1f(const std::string& name, float value) {
    GLCall(glUniform1f(GetUniformLocation(name), value))
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3))
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]))
}

void Shader::SetUniform2f(const std::string& name, const glm::vec2& vec) {
    GLCall(glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(vec)))
}

int Shader::GetUniformLocation(const std::string& name) {
    if (UniformLocationCache_.find(name) != UniformLocationCache_.end())
        return UniformLocationCache_[name];
    
    GLCall(int location = glGetUniformLocation(RendererID_, name.c_str()))
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    UniformLocationCache_[name] = location;
    
    return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) { // can be optimized
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
        
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // returns pointer to beginning of data
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    const unsigned int program = glCreateProgram();
    const unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    const unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDetachShader(program, vs);
    glDetachShader(program, fs);

    return program;
}

Shader::~Shader() {
    GLCall(glDeleteProgram(RendererID_))
}
