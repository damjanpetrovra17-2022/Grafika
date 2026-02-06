#include "../Header/SeatGrid.h"
#include "../Header/DebugCube.h"
#include "../Header/Light.h"
#include "../Shader.h"
#include <glm/gtc/matrix_transform.hpp>

SeatGrid::SeatGrid()
    : m_cubeMesh(nullptr)
    , m_origin(0.0f)
    , m_seatSpacingX(1.0f)
    , m_seatSpacingZ(1.2f)
    , m_rowElevationStep(0.3f)
    , m_seatHalfExtents(0.4f, 0.45f, 0.4f)  // width=0.8m, height=0.9m, depth=0.8m
{
}

SeatGrid::~SeatGrid()
{
}

void SeatGrid::init(
    DebugCube* cubeMesh,
    const glm::vec3& origin,
    float seatSpacingX,
    float seatSpacingZ,
    float rowElevationStep)
{
    m_cubeMesh = cubeMesh;
    m_origin = origin;
    m_seatSpacingX = seatSpacingX;
    m_seatSpacingZ = seatSpacingZ;
    m_rowElevationStep = rowElevationStep;
    
    // Seat dimensions (chair-like proportions)
    // Width: 0.8m, Height: 0.9m, Depth: 0.8m
    m_seatHalfExtents = glm::vec3(0.4f, 0.45f, 0.4f);
    
    // Create platforms first (continuous stepped floor)
    createPlatforms();
    
    // Then create seats on top of platforms
    createSeats();
}

void SeatGrid::createPlatforms()
{
    m_platforms.clear();
    m_platforms.reserve(ROWS);
    
    // Platform dimensions - FIXED to extend from floor to platform top
    const float platformWidth = (COLS * m_seatSpacingX) + 4.0f;  // Wider for walking space
    const float walkingSpace = 1.0f;    // Increased walking space (was 0.7m)
    const float seatDepth = m_seatHalfExtents.z * 2.0f;  // Full seat depth (0.8m)
    const float platformDepth = seatDepth + walkingSpace * 2.0f;  // Front + back walking space
    
    const float floorY = 0.5f;  // Scene floor level
    
    // Create one platform per row - each extends from floor to its top
    for (int row = 0; row < ROWS; ++row)
    {
        // Platform TOP Y position
        float platformTopY = m_origin.y + row * m_rowElevationStep;
        
        // Platform extends from FLOOR to TOP (no floating)
        float platformHeight = platformTopY - floorY + 0.1f;  // From floor to platform top
        
        // Platform Z position (centered on row)
        float platformZ = m_origin.z + row * m_seatSpacingZ;
        
        // Platform center position (halfway between floor and top)
        glm::vec3 platformPos(
            m_origin.x,  // Centered on X
            floorY + platformHeight * 0.5f,  // Center Y (between floor and top)
            platformZ  // Centered on row Z
        );
        
        glm::vec3 platformSize(platformWidth, platformHeight, platformDepth);
        
        m_platforms.emplace_back(platformPos, platformSize);
    }
}


void SeatGrid::createSeats()
{
    // Create grid of seats on top of platforms
    // Left section: cols 0-4, Right section: cols 5-9
    // Seats are offset to leave walking space in front
    
    const float walkingSpace = 1.0f;  // Walking aisle depth (increased)
    const float seatOffsetZ = 0.0f;  // Seats centered on platform
    
    for (int row = 0; row < ROWS; ++row)
    {
        // Get platform for this row
        const StepPlatform& platform = m_platforms[row];
        
        // Platform top Y is at position.y + size.y/2
        float platformTopY = platform.position.y + platform.size.y * 0.5f;
        
        for (int col = 0; col < COLS; ++col)
        {
            // Seat position calculation
            glm::vec3 position;
            
            // Shift seats to create gap in middle for stairs
            // Left section (cols 0-4): shift left by 1.5m
            // Right section (cols 5-9): shift right by 1.5m
            float xOffset = (col < 5) ? -1.5f : 1.5f;
            
            position.x = m_origin.x + (col - COLS / 2.0f + 0.5f) * m_seatSpacingX + xOffset;
            position.z = m_origin.z + row * m_seatSpacingZ + seatOffsetZ;
            
            // Place seat ON TOP of platform (seat bottom at platform top)
            position.y = platformTopY + m_seatHalfExtents.y;
            
            // Compute AABB bounds for this seat (for ray picking, NOT collision)
            AABB bounds(
                position - m_seatHalfExtents,
                position + m_seatHalfExtents
            );
            
            // Create seat
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
    
    // Set lighting (room light)
    phongShader->setVec3("lightPos", glm::vec3(0.0f, 4.0f, 0.0f));
    phongShader->setVec3("lightColor", glm::vec3(1.0f, 0.95f, 0.85f));
    phongShader->setFloat("lightIntensity", 5.0f);
    glUniform1i(glGetUniformLocation(phongShader->ID, "lightEnabled"), 1);
    
    // 1. Draw platforms first (stepped floor)
    const glm::vec3 platformColor(0.35f, 0.3f, 0.25f);  // Medium brown
    
    for (const auto& platform : m_platforms)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, platform.position);
        model = glm::scale(model, platform.size);
        
        phongShader->setMat4("model", model);
        phongShader->setVec3("uBaseColor", platformColor);
        
        m_cubeMesh->draw();
    }
    
    // 2. Draw all seats on top of platforms
    for (int row = 0; row < ROWS; ++row)
    {
        for (int col = 0; col < COLS; ++col)
        {
            const Seat& seat = m_seats[row][col];
            
            // Determine color based on state
            glm::vec3 seatColor;
            switch (seat.state)
            {
                case SeatState::Free:
                    seatColor = glm::vec3(0.2f, 0.7f, 0.2f);  // Green
                    break;
                case SeatState::Reserved:
                    seatColor = glm::vec3(0.9f, 0.9f, 0.2f);  // Yellow
                    break;
                case SeatState::Purchased:
                    seatColor = glm::vec3(0.8f, 0.2f, 0.2f);  // Red
                    break;
            }
            
            // Build model matrix for this seat
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, seat.position);
            model = glm::scale(model, m_seatHalfExtents * 2.0f);  // Scale to full size
            
            phongShader->setMat4("model", model);
            phongShader->setVec3("uBaseColor", seatColor);
            
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
    // Validate group size
    if (N < 1 || N > 9)
        return false;  // Invalid group size
    
    if (N > COLS)
        return false;  // Group too large for any row
    
    // SCAN ORDER (CRITICAL):
    // 1. Start from LAST row (ROWS-1) toward FIRST row (0)
    // 2. Within each row, scan from RIGHTMOST (COLS-1) toward LEFT
    // 3. Find FIRST block of N consecutive Free seats
    
    for (int row = ROWS - 1; row >= 0; --row)
    {
        // Scan from right to left, looking for N consecutive Free seats
        // We need to check positions [col, col-1, ..., col-(N-1)]
        // So col must be >= (N-1) to have enough seats to the left
        
        for (int col = COLS - 1; col >= N - 1; --col)
        {
            // Check if N consecutive seats starting from col going left are all Free
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
            
            // Found a valid block!
            if (allFree)
            {
                // Purchase all N seats
                for (int i = 0; i < N; ++i)
                {
                    int purchaseCol = col - i;
                    m_seats[row][purchaseCol].state = SeatState::Purchased;
                }
                
                // Success - group is seated together
                return true;
            }
        }
    }
    
    // No valid block found in entire grid
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


