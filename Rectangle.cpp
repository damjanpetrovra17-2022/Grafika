#include "Rectangle.h"
#include "Header/stb_image.h"
#include <iostream>

Rectangle::Rectangle(float x, float y, float width, float height, Shader* shader)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->shader = shader;

    r = g = b = 1.0f;
    a = 1.0f;

    float vertices[] =
    {
        // pos      // uv
        -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  1.0f, 1.0f,

        -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // uv coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Rectangle::setTexture(const char* filename)
{
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int w, h, channels;
    stbi_set_flip_vertically_on_load(true);
    
    unsigned char* data = stbi_load(filename, &w, &h, &channels, 4);

    if (!data)
    {
        std::cout << "Failed to load texture: " << filename << std::endl;
        return;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
    hasTexture = true;
}

void Rectangle::draw()
{
    shader->use();

    shader->setVec3("uPosSize", x, y, width);
    shader->setFloat("uHeight", height);
    shader->setVec4("uColor", r, g, b, a);

    if (hasTexture)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // pošto nemaš setBool → koristiš float uniform
        shader->setFloat("useTexture", 1.0f);

        // pošto nemaš setInt → koristiš direktno OpenGL uniform call
        glUniform1i(glGetUniformLocation(shader->ID, "tex"), 0);
    }
    else
    {
        shader->setFloat("useTexture", 0.0f);
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

