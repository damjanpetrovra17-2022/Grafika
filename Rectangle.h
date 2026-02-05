#pragma once
#include <GL/glew.h>
#include "Shader.h"

class Rectangle
{
public:
    float x, y;
    float width, height;

    float r, g, b, a;

    Rectangle(float x, float y, float width, float height, Shader* shader);

    void setColor(float R, float G, float B, float A = 1.0f)
    {
        r = R; g = G; b = B; a = A;
        hasTexture = false;  // kada koristiš boju, nema teksture
    }

    void setTexture(const char* filename);

    void draw();

private:
    unsigned int VAO, VBO;
    unsigned int textureID = 0;
    bool hasTexture = false;

    Shader* shader;
};
