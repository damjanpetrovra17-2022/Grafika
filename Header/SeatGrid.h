#pragma once

#include "Seat.h"
#include "AABB.h"
#include <glm/glm.hpp>
#include <vector>

class Shader;
class DebugCube;

// Stepped platform for stadium seating
// Each row has one continuous platform that seats rest on
struct StepPlatform
{
    glm::vec3 position;     // Center position
    glm::vec3 size;         // Full size (width, height, depth)
    AABB bounds;            // Collision bounds
    
    StepPlatform()
        : position(0.0f)
        , size(1.0f)
        , bounds(glm::vec3(0.0f), glm::vec3(1.0f))
    {
    }
    
    StepPlatform(const glm::vec3& pos, const glm::vec3& sz)
        : position(pos)
        , size(sz)
    {
        glm::vec3 halfExtents = size * 0.5f;
        bounds = AABB(position - halfExtents, position + halfExtents);
    }
};

class SeatGrid
{
public:
    static constexpr int ROWS = 5;
    static constexpr int COLS = 10;
    
    SeatGrid();
    ~SeatGrid();
    
    void init(
        DebugCube* cubeMesh,
        const glm::vec3& origin,
        float seatSpacingX,
        float seatSpacingZ,
        float rowElevationStep
    );
    
    void draw(Shader* phongShader, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos);
    
    Seat* getSeat(int row, int col);
    const Seat* getSeat(int row, int col) const;
    
    // Phase 7: Purchase N adjacent seats (group seating)
    // Returns true if successful, false if no valid block found
    // Groups MUST be in same row, fully adjacent (no gaps)
    bool purchaseAdjacent(int N);
    
    // Get platform collision bounds for camera
    std::vector<AABB> getPlatformBounds() const;
    
private:
    Seat m_seats[ROWS][COLS];
    std::vector<StepPlatform> m_platforms;  // One platform per row
    DebugCube* m_cubeMesh;  // Shared mesh (not owned)
    
    glm::vec3 m_origin;
    float m_seatSpacingX;
    float m_seatSpacingZ;
    float m_rowElevationStep;
    glm::vec3 m_seatHalfExtents;  // Half-size of seat cuboid for AABB
    
    void createPlatforms();
    void createSeats();
};
