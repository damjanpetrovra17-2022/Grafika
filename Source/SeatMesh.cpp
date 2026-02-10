#include "../Header/SeatMesh.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <limits>

SeatMesh::SeatMesh()
    : m_VAO(0)
    , m_VBO(0)
    , m_vertexCount(0)
    , m_initialized(false)
{
}

SeatMesh::~SeatMesh()
{
    cleanup();
}

bool SeatMesh::loadOBJ(const std::string& path)
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
                }
            }
        }
    }
    file.close();

    m_vertexCount = (int)(verts.size() / 6);
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
        int base = i * 6;
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
        int base = i * 6;
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
                          6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    m_initialized = true;
    std::cout << "[INFO] Loaded seat mesh: " << path
              << " (" << m_vertexCount << " verts)" << std::endl;
    return true;
}

void SeatMesh::draw() const
{
    if (!m_initialized) return;
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    glBindVertexArray(0);
}

void SeatMesh::cleanup()
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
}
