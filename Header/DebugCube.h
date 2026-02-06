#pragma once

#include <GL/glew.h>

class DebugCube
{
public:
    DebugCube();
    ~DebugCube();

    void init();
    void draw();
    void cleanup();

private:
    GLuint m_VAO;
    GLuint m_VBO;
    bool m_initialized;
};
