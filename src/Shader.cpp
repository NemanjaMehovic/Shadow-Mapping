#include<Shader.h>

#include<iostream>
#include<fstream>
#include<sstream>

#include<glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Shader::Shader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
{
    shaderId = glCreateProgram();
    unsigned int vertexShader = createShader(vertexShaderFile, GL_VERTEX_SHADER);
    unsigned int fragmentShader = createShader(fragmentShaderFile, GL_FRAGMENT_SHADER);
    glAttachShader(shaderId, vertexShader);
    glAttachShader(shaderId, fragmentShader);
    glLinkProgram(shaderId);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
	glDeleteProgram(shaderId);
}

void Shader::activate() const
{
    glUseProgram(shaderId);
}

void Shader::setMat3(const glm::mat3& transform, const std::string& name) const
{
    unsigned int transformLoc = glGetUniformLocation(shaderId, name.c_str());
    glUniformMatrix3fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}

void Shader::setMat4(const glm::mat4& transform, const std::string& name) const
{
    unsigned int transformLoc = glGetUniformLocation(shaderId, name.c_str());
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}

void Shader::setVec3(const glm::vec3& vec3, const std::string& name) const
{
    unsigned int transformLoc = glGetUniformLocation(shaderId, name.c_str());
    glUniform3fv(transformLoc, 1, glm::value_ptr(vec3));
}

void Shader::setBool(const bool& value, const std::string& name) const
{
    unsigned int location = glGetUniformLocation(shaderId, name.c_str());
    glUniform1i(location, value);
}

void Shader::setInt(const int& value, const std::string& name) const
{
    unsigned int location = glGetUniformLocation(shaderId, name.c_str());
    glUniform1i(location, value);
}

unsigned int Shader::createShader(const std::string& shaderFileName, const unsigned int& shaderType) const
{
    unsigned int id;
    id = glCreateShader(shaderType);

    std::string fileData = readShaderFileData(shaderFileName);
    const GLchar* shaderSource = fileData.c_str();

    glShaderSource(id, 1, &shaderSource, NULL);
    glCompileShader(id);
    checkShaderCompileError(id, shaderFileName);

    return id;
}

void Shader::checkShaderCompileError(const unsigned int& id, const std::string& shaderName) const
{
    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << shaderName << "::COMPILATION_FAILED" << std::endl << infoLog << std::endl;
    }
}

void Shader::checkShaderLinkerError() const
{
    int success;
    char infoLog[512];
    glGetProgramiv(shaderId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderId, 512, NULL, infoLog);
        std::cout << "ERROR::SHADERPROGRAM::LINKING" << std::endl << infoLog << std::endl;
    }
}

std::string Shader::readShaderFileData(const std::string& shaderFileName) const
{
    std::ifstream fileStream;
    std::stringstream stream;
    std::string data;

    fileStream.open(shaderFileName);

    if (fileStream.is_open())
    {
        stream << fileStream.rdbuf();
        data = stream.str();
    }
    else
        std::cout << "Error failed to open " << shaderFileName << std::endl;

    fileStream.close();
    return data;
}

