#include "../Header/SeatGrid.h"
#include "../Header/DebugCube.h"
#include "../Header/SeatMesh.h"
#include "../Header/Light.h"
#include "../Shader.h"
#include <glm/gtc/matrix_transform.hpp>

SeatGrid::SeatGrid()
    : m_cubeMesh(nullptr)
    , m_seatMesh(nullptr)
    , m_origin(0.0f)
    , m_seatSpacingX(1.0f)
    , m_seatSpacingZ(1.2f)
    , m_rowElevationStep(0.3f)
    , m_seatHalfExtents(0.5f, 0.55f, 0.5f)  
{
}

SeatGrid::~SeatGrid()
{
}

void SeatGrid::init(
    DebugCube* cubeMesh,
    SeatMesh* seatMesh,
    const glm::vec3& origin,
    float seatSpacingX,
    float seatSpacingZ,
    float rowElevationStep)
{
    m_cubeMesh = cubeMesh;
    m_seatMesh = seatMesh;
    m_origin = origin;
    m_seatSpacingX = seatSpacingX;
    m_seatSpacingZ = seatSpacingZ;
    m_rowElevationStep = rowElevationStep;
    
    
    
    m_seatHalfExtents = glm::vec3(0.5f, 0.55f, 0.5f);
    
    
    createPlatforms();
    
    
    createSeats();
}

void SeatGrid::createPlatforms()
{
    m_platforms.clear();
    m_platforms.reserve(ROWS);
    
    
    const float platformWidth = (COLS * m_seatSpacingX) + 4.0f;  
    const float walkingSpace = 1.0f;    
    const float seatDepth = m_seatHalfExtents.z * 2.0f;  
    const float platformDepth = seatDepth + walkingSpace * 2.0f;  
    
    const float floorY = 0.5f;  
    
    
    for (int row = 0; row < ROWS; ++row)
    {
        
        float platformTopY = m_origin.y + row * m_rowElevationStep;
        
        
        float platformHeight = platformTopY - floorY + 0.1f;  
        
        
        float platformZ = m_origin.z + row * m_seatSpacingZ;
        
        
        glm::vec3 platformPos(
            m_origin.x,  
            floorY + platformHeight * 0.5f,  
            platformZ  
        );
        
        glm::vec3 platformSize(platformWidth, platformHeight, platformDepth);
        
        m_platforms.emplace_back(platformPos, platformSize);
    }
}

void SeatGrid::createSeats()
{
    
    
    
    
    const float walkingSpace = 1.0f;  
    const float seatOffsetZ = 0.0f;  
    
    for (int row = 0; row < ROWS; ++row)
    {
        
        const StepPlatform& platform = m_platforms[row];
        
        
        float platformTopY = platform.position.y + platform.size.y * 0.5f;
        
        for (int col = 0; col < COLS; ++col)
        {
            
            glm::vec3 position;
            
            
            
            
            float xOffset = (col < 5) ? -1.5f : 1.5f;
            
            position.x = m_origin.x + (col - COLS / 2.0f + 0.5f) * m_seatSpacingX + xOffset;
            position.z = m_origin.z + row * m_seatSpacingZ + seatOffsetZ;
            
            
            position.y = platformTopY + m_seatHalfExtents.y;
            
            
            AABB bounds(
                position - m_seatHalfExtents,
                position + m_seatHalfExtents
            );
            
            
            m_seats[row][col] = Seat(row, col, position, bounds);
        }
    }
}

void SeatGrid::draw(Shader* phongShader, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos)
{
    if (!m_cubeMesh || !phongShader)
        return;
    
    phongShader->use();
    phongShader->setMat4("view", view);
    phongShader->setMat4("projection", projection);
    phongShader->setVec3("viewPos", viewPos);
    
    
    phongShader->setVec3("lightPos", glm::vec3(0.0f, 4.0f, 0.0f));
    phongShader->setVec3("lightColor", glm::vec3(1.0f, 0.95f, 0.85f));
    phongShader->setFloat("lightIntensity", 5.0f);
    glUniform1i(glGetUniformLocation(phongShader->ID, "lightEnabled"), 1);
    
    
    const glm::vec3 platformColor(0.35f, 0.3f, 0.25f);  
    
    for (const auto& platform : m_platforms)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, platform.position);
        model = glm::scale(model, platform.size);
        
        phongShader->setMat4("model", model);
        phongShader->setVec3("uBaseColor", platformColor);
        
        m_cubeMesh->draw();
    }
    
    
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            const Seat& seat = m_seats[row][col];
            
            
            glm::vec3 seatColor;
            switch (seat.state)
            {
                case SeatState::Free:
                    seatColor = glm::vec3(0.2f, 0.7f, 0.2f);  
                    break;
                case SeatState::Reserved:
                    seatColor = glm::vec3(0.9f, 0.9f, 0.2f);  
                    break;
                case SeatState::Purchased:
                    seatColor = glm::vec3(0.8f, 0.2f, 0.2f);  
                    break;
            }
            
            
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, seat.position);
            model = glm::rotate(model, glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));  
            model = glm::scale(model, m_seatHalfExtents * 2.0f);  
            
            phongShader->setMat4("model", model);
            phongShader->setVec3("uBaseColor", seatColor);
            
            
            if (m_seatMesh)
                m_seatMesh->draw();
            else
                m_cubeMesh->draw();
        }
    }
}

Seat* SeatGrid::getSeat(int row, int col)
{
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
        return nullptr;
    
    return &m_seats[row][col];
}

const Seat* SeatGrid::getSeat(int row, int col) const
{
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
        return nullptr;
    
    return &m_seats[row][col];
}

bool SeatGrid::purchaseAdjacent(int N)
{
    
    if (N < 1 || N > 9)
        return false;  
    
    if (N > COLS)
        return false;  
    
    
    
    
    
    
    for (int row = ROWS - 1; row >= 0; --row)
    {
        
        
        
        
        for (int col = COLS - 1; col >= N - 1; --col)
        {
            
            bool allFree = true;
            
            for (int i = 0; i < N; ++i)
            {
                int checkCol = col - i;
                if (m_seats[row][checkCol].state != SeatState::Free)
                {
                    allFree = false;
                    break;
                }
            }
            
            
            if (allFree)
            {
                
                for (int i = 0; i < N; ++i)
                {
                    int purchaseCol = col - i;
                    m_seats[row][purchaseCol].state = SeatState::Purchased;
                }
                
                
                return true;
            }
        }
    }
    
    
    return false;
}

std::vector<AABB> SeatGrid::getPlatformBounds() const
{
    std::vector<AABB> bounds;
    bounds.reserve(m_platforms.size());
    
    for (const auto& platform : m_platforms)
    {
        bounds.push_back(platform.bounds);
    }
    
    return bounds;
}

