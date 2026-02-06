#pragma once

#include <glm/glm.hpp>

// Forward declaration
struct Seat;

// Movement stages for person animation
enum class MovementStage
{
    ToRowDepth,       // Stage 1: Move along aisle to target row Z
    ClimbToRowHeight, // Stage 2: Climb to target row Y
    ToSeatX,          // Stage 3: Move across row to seat X
    Seated,           // Stage 4: Seated at target
    Exited            // Stage 5: Exited through door (for Phase 10)
};

// Travel direction
enum class TravelMode
{
    Entering,  // Moving from door to seat
    Exiting    // Moving from seat to door (Phase 10)
};

class Person
{
public:
    Person();
    Person(const glm::vec3& doorPos, Seat* targetSeat, const glm::vec3& baseColor);
    
    void update(float deltaTime);
    
    // Accessors
    const glm::vec3& getPosition() const { return m_position; }
    const glm::vec3& getColor() const { return m_baseColor; }
    MovementStage getStage() const { return m_stage; }
    bool isSeated() const { return m_stage == MovementStage::Seated; }
    bool isExited() const { return m_stage == MovementStage::Exited; }
    
    // For Phase 10 - exit functionality
    void startExiting();
    
private:
    glm::vec3 m_position;      // Current world position
    glm::vec3 m_doorPos;       // Door spawn/exit position
    Seat* m_targetSeat;        // Target seat (nullptr if none)
    
    MovementStage m_stage;
    TravelMode m_mode;
    
    float m_speed;             // Movement speed (units/sec)
    glm::vec3 m_baseColor;     // Visual color for differentiation
    
    // Movement thresholds
    static constexpr float EPSILON = 0.05f;
    static constexpr float AISLE_X = -7.0f;  // Aisle position (left side of hall)
    
    void updateEntering(float deltaTime);
    void updateExiting(float deltaTime);
    
    bool moveToward(float& current, float target, float speed, float dt);
};
