#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
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

    void setInt(const std::string& name, int value)
    {
        GLint loc = glGetUniformLocation(ID, name.c_str());
        glUniform1i(loc, value);
    }

    void setVec3(const std::string& name, float x, float y, float z)
    {
        GLint loc = glGetUniformLocation(ID, name.c_str());
        glUniform3f(loc, x, y, z);
    }

    void setVec3(const std::string& name, const glm::vec3& value)
    {
        GLint loc = glGetUniformLocation(ID, name.c_str());
        glUniform3fv(loc, 1, glm::value_ptr(value));
    }

    void setVec4(const std::string& name, float x, float y, float z, float w)
    {
        GLint loc = glGetUniformLocation(ID, name.c_str());
        glUniform4f(loc, x, y, z, w);
    }

    void setMat4(const std::string& name, const glm::mat4& mat)
    {
        GLint loc = glGetUniformLocation(ID, name.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
    }
};
