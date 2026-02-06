#pragma once

#include <GL/glew.h>

class Shader;

class HUD
{
public:
    HUD();
    ~HUD();

    void init(int screenWidth, int screenHeight);
    void draw();
    void shutdown();

private:
    void loadTexture(const char* path);
    void createQuad();

    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_texture;
    Shader* m_shader;
    
    int m_screenWidth;
    int m_screenHeight;
    
    bool m_initialized;
};
