#include "../Header/HumanMesh.h"
#include "../Header/stb_image.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <limits>

HumanMesh::HumanMesh()
    : m_VAO(0)
    , m_VBO(0)
    , m_vertexCount(0)
    , m_initialized(false)
    , m_textureID(0)
{
}

HumanMesh::~HumanMesh()
{
    cleanup();
}

bool HumanMesh::loadOBJ(const std::string& path)
{
    if (m_initialized) return true;

    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "[ERROR] Cannot open OBJ: " << path << std::endl;
        return false;
    }

    std::vector<float> posX, posY, posZ;
    std::vector<float> nrmX, nrmY, nrmZ;
    std::vector<float> texU, texV;

    
    std::vector<float> verts;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.size() < 2) continue;

        if (line[0] == 'v' && line[1] == ' ')
        {
            float x, y, z;
            if (sscanf(line.c_str(), "v %f %f %f", &x, &y, &z) == 3)
            {
                posX.push_back(x);
                posY.push_back(y);
                posZ.push_back(z);
            }
        }
        else if (line[0] == 'v' && line[1] == 'n')
        {
            float x, y, z;
            if (sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z) == 3)
            {
                nrmX.push_back(x);
                nrmY.push_back(y);
                nrmZ.push_back(z);
            }
        }
        else if (line[0] == 'v' && line[1] == 't')
        {
            float u, v;
            if (sscanf(line.c_str(), "vt %f %f", &u, &v) >= 2)
            {
                texU.push_back(u);
                texV.push_back(v);
            }
        }
        else if (line[0] == 'f' && line[1] == ' ')
        {
            
            int vi[3], ti[3], ni[3];
            if (sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
                       &vi[0], &ti[0], &ni[0],
                       &vi[1], &ti[1], &ni[1],
                       &vi[2], &ti[2], &ni[2]) == 9)
            {
                for (int k = 0; k < 3; ++k)
                {
                    int pi = vi[k] - 1;
                    int tIdx = ti[k] - 1;
                    int nIdx = ni[k] - 1;

                    verts.push_back(posX[pi]);
                    verts.push_back(posY[pi]);
                    verts.push_back(posZ[pi]);

                    if (nIdx >= 0 && nIdx < (int)nrmX.size())
                    {
                        verts.push_back(nrmX[nIdx]);
                        verts.push_back(nrmY[nIdx]);
                        verts.push_back(nrmZ[nIdx]);
                    }
                    else
                    {
                        verts.push_back(0.0f);
                        verts.push_back(1.0f);
                        verts.push_back(0.0f);
                    }

                    if (tIdx >= 0 && tIdx < (int)texU.size())
                    {
                        verts.push_back(texU[tIdx]);
                        verts.push_back(texV[tIdx]);
                    }
                    else
                    {
                        verts.push_back(0.0f);
                        verts.push_back(0.0f);
                    }
                }
            }
        }
    }
    file.close();

    m_vertexCount = (int)(verts.size() / 8);
    if (m_vertexCount == 0)
    {
        std::cerr << "[ERROR] OBJ produced 0 vertices: " << path << std::endl;
        return false;
    }

    
    float minX =  (std::numeric_limits<float>::max)();
    float minY =  (std::numeric_limits<float>::max)();
    float minZ =  (std::numeric_limits<float>::max)();
    float maxX = -(std::numeric_limits<float>::max)();
    float maxY = -(std::numeric_limits<float>::max)();
    float maxZ = -(std::numeric_limits<float>::max)();

    for (int i = 0; i < m_vertexCount; ++i)
    {
        int base = i * 8;
        float x = verts[base + 0];
        float y = verts[base + 1];
        float z = verts[base + 2];
        if (x < minX) minX = x;  if (x > maxX) maxX = x;
        if (y < minY) minY = y;  if (y > maxY) maxY = y;
        if (z < minZ) minZ = z;  if (z > maxZ) maxZ = z;
    }

    float cx = (minX + maxX) * 0.5f;
    float cy = (minY + maxY) * 0.5f;
    float cz = (minZ + maxZ) * 0.5f;

    float sx = maxX - minX;
    float sy = maxY - minY;
    float sz = maxZ - minZ;
    float maxSpan = sx;
    if (sy > maxSpan) maxSpan = sy;
    if (sz > maxSpan) maxSpan = sz;
    if (maxSpan < 0.0001f) maxSpan = 1.0f;

    float invSpan = 1.0f / maxSpan;

    for (int i = 0; i < m_vertexCount; ++i)
    {
        int base = i * 8;
        verts[base + 0] = (verts[base + 0] - cx) * invSpan;
        verts[base + 1] = (verts[base + 1] - cy) * invSpan;
        verts[base + 2] = (verts[base + 2] - cz) * invSpan;
    }

    
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)(verts.size() * sizeof(float)),
                 verts.data(), GL_STATIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float),
                          (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    m_initialized = true;
    std::cout << "[INFO] Loaded human mesh: " << path
              << " (" << m_vertexCount << " verts)" << std::endl;
    return true;
}

bool HumanMesh::loadTexture(const std::string& texPath)
{
    if (m_textureID != 0)
        return true;

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);

    if (!data)
    {
        std::cerr << "[ERROR] Cannot load texture: " << texPath << std::endl;
        return false;
    }

    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    std::cout << "[INFO] Loaded human texture: " << texPath
              << " (" << width << "x" << height << ")" << std::endl;
    return true;
}

bool HumanMesh::loadMultipleTextures(const std::string& basePath, int count)
{
    m_textureIDs.clear();
    
    int loadedCount = 0;
    for (int i = 1; i <= count; ++i)
    {
        std::string texPath = basePath + "/human" + std::to_string(i) + ".png";
        
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(texPath.c_str(), &width, &height, &nrChannels, 0);
        
        if (!data)
        {
            std::cerr << "[WARNING] Cannot load texture: " << texPath << " (skipping)" << std::endl;
            continue;
        }
        
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        
        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        m_textureIDs.push_back(texID);
        loadedCount++;
        std::cout << "[INFO] Loaded human texture: " << texPath
                  << " (" << width << "x" << height << ")" << std::endl;
    }
    
    if (loadedCount == 0)
    {
        std::cerr << "[ERROR] Failed to load any human textures from: " << basePath << std::endl;
        return false;
    }
    
    std::cout << "[INFO] Successfully loaded " << loadedCount << " out of " << count << " human textures" << std::endl;
    return true;
}

GLuint HumanMesh::getTextureID(int index) const
{
    if (index >= 0 && index < (int)m_textureIDs.size())
        return m_textureIDs[index];
    return m_textureID;
}

void HumanMesh::draw() const
{
    if (!m_initialized) return;
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    glBindVertexArray(0);
}

void HumanMesh::cleanup()
{
    if (m_initialized)
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        m_VAO = 0;
        m_VBO = 0;
        m_vertexCount = 0;
        m_initialized = false;
    }
    if (m_textureID != 0)
    {
        glDeleteTextures(1, &m_textureID);
        m_textureID = 0;
    }
    if (!m_textureIDs.empty())
    {
        glDeleteTextures((GLsizei)m_textureIDs.size(), m_textureIDs.data());
        m_textureIDs.clear();
    }
}
