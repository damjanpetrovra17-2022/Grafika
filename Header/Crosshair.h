#pragma once

class Shader;

class Crosshair
{
public:
    Crosshair();
    ~Crosshair();
    
    void init();
    void draw(Shader* shader, int screenWidth, int screenHeight);
    void cleanup();
    
private:
    unsigned int m_VAO;
    unsigned int m_VBO;
    bool m_initialized;
};
