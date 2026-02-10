#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>

class HumanMesh
{
public:
    HumanMesh();
    ~HumanMesh();

    bool loadOBJ(const std::string& objPath);
    bool loadTexture(const std::string& texPath);
    bool loadMultipleTextures(const std::string& basePath, int count);
    void draw() const;
    void cleanup();

    GLuint getTextureID() const { return m_textureID; }
    GLuint getTextureID(int index) const;
    int getTextureCount() const { return (int)m_textureIDs.size(); }

private:
    GLuint m_VAO;
    GLuint m_VBO;
    int    m_vertexCount;
    bool   m_initialized;

    GLuint m_textureID;
    std::vector<GLuint> m_textureIDs;
};
