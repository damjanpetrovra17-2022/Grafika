#include "Shader.h"
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    
    std::ifstream vFile(vertexPath);
    std::ifstream fFile(fragmentPath);

    if (!vFile.is_open())
    {
        std::cerr << "ERROR: Failed to open vertex shader file: " << vertexPath << std::endl;
        ID = 0;
        return;
    }

    if (!fFile.is_open())
    {
        std::cerr << "ERROR: Failed to open fragment shader file: " << fragmentPath << std::endl;
        ID = 0;
        return;
    }

    std::stringstream vStream, fStream;
    vStream << vFile.rdbuf();
    fStream << fFile.rdbuf();

    vFile.close();
    fFile.close();

    std::string vCode = vStream.str();
    std::string fCode = fStream.str();

    const char* vShaderCode = vCode.c_str();
    const char* fShaderCode = fCode.c_str();

    
    unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    
    int success;
    char infoLog[512];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cerr << "ERROR: Vertex shader compilation failed\n" << infoLog << std::endl;
    }
    else
    {
        std::cout << "Vertex shader compiled successfully" << std::endl;
    }

    
    unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cerr << "ERROR: Fragment shader compilation failed\n" << infoLog << std::endl;
    }
    else
    {
        std::cout << "Fragment shader compiled successfully" << std::endl;
    }

    
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cerr << "ERROR: Shader program linking failed\n" << infoLog << std::endl;
    }
    else
    {
        std::cout << "Shader program linked successfully" << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
