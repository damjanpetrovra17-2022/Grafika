#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use()
    {
        glUseProgram(ID);
    }

    void setFloat(const std::string& name, float value)
    {
        GLint loc = glGetUniformLocation(ID, name.c_str());
        glUniform1f(loc, value);
    }

    void setVec3(const std::string& name, float x, float y, float z)
    {
        GLint loc = glGetUniformLocation(ID, name.c_str());
        glUniform3f(loc, x, y, z);
    }

    void setVec4(const std::string& name, float x, float y, float z, float w)
    {
        GLint loc = glGetUniformLocation(ID, name.c_str());
        glUniform4f(loc, x, y, z, w);
    }
};
