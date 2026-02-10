#pragma once

#include <GL/glew.h>
#include <string>

class SeatMesh
{
public:
    SeatMesh();
    ~SeatMesh();

    bool loadOBJ(const std::string& path);
    void draw() const;
    void cleanup();

private:
    GLuint m_VAO;
    GLuint m_VBO;
    int    m_vertexCount;
    bool   m_initialized;
};
