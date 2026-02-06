#include "../Header/Person.h"
#include "../Header/Seat.h"
#include <algorithm>

Person::Person()
    : m_position(0.0f)
    , m_doorPos(0.0f)
    , m_targetSeat(nullptr)
    , m_stage(MovementStage::ToRowDepth)
    , m_mode(TravelMode::Entering)
    , m_speed(2.0f)
    , m_baseColor(1.0f)
{
}

Person::Person(const glm::vec3& doorPos, Seat* targetSeat, const glm::vec3& baseColor)
    : m_position(doorPos)
    , m_doorPos(doorPos)
    , m_targetSeat(targetSeat)
    , m_stage(MovementStage::ToRowDepth)
    , m_mode(TravelMode::Entering)
    , m_speed(2.0f)
    , m_baseColor(baseColor)
{
}

void Person::update(float deltaTime)
{
    if (m_stage == MovementStage::Seated || m_stage == MovementStage::Exited)
        return;  // No movement needed
    
    if (m_mode == TravelMode::Entering)
    {
        updateEntering(deltaTime);
    }
    else
    {
        updateExiting(deltaTime);
    }
}

void Person::updateEntering(float deltaTime)
{
    if (!m_targetSeat)
        return;
    
    const glm::vec3& seatPos = m_targetSeat->position;
    
    switch (m_stage)
    {
        case MovementStage::ToRowDepth:
        {
            // Stage 1: Move along aisle (Z) to target row depth
            // Keep X at aisle position, keep Y near current
            m_position.x = AISLE_X;
            
            if (moveToward(m_position.z, seatPos.z, m_speed, deltaTime))
            {
                m_stage = MovementStage::ClimbToRowHeight;
            }
            break;
        }
        
        case MovementStage::ClimbToRowHeight:
        {
            // Stage 2: Climb to target row height (Y)
            // Stay at current X and Z
            if (moveToward(m_position.y, seatPos.y, m_speed, deltaTime))
            {
                m_stage = MovementStage::ToSeatX;
            }
            break;
        }
        
        case MovementStage::ToSeatX:
        {
            // Stage 3: Move across row to seat X
            // Keep Y and Z stable
            if (moveToward(m_position.x, seatPos.x, m_speed, deltaTime))
            {
                // Snap to final seat position with slight offset (sit on seat)
                const float personHeight = 1.7f;
                m_position = seatPos + glm::vec3(0.0f, personHeight * 0.5f, 0.0f);
                m_stage = MovementStage::Seated;
            }
            break;
        }
        
        default:
            break;
    }
}

void Person::updateExiting(float deltaTime)
{
    // Phase 10 - reverse path
    // For now, just a stub structure
    
    switch (m_stage)
    {
        case MovementStage::Seated:
        {
            // Start exit - move back to aisle
            m_stage = MovementStage::ToSeatX;  // Reuse stages in reverse
            break;
        }
        
        case MovementStage::ToSeatX:
        {
            // Move from seat X back to aisle X
            if (moveToward(m_position.x, AISLE_X, m_speed, deltaTime))
            {
                m_stage = MovementStage::ClimbToRowHeight;
            }
            break;
        }
        
        case MovementStage::ClimbToRowHeight:
        {
            // Move down to door height
            if (moveToward(m_position.y, m_doorPos.y, m_speed, deltaTime))
            {
                m_stage = MovementStage::ToRowDepth;
            }
            break;
        }
        
        case MovementStage::ToRowDepth:
        {
            // Move along aisle back to door Z
            m_position.x = AISLE_X;
            
            if (moveToward(m_position.z, m_doorPos.z, m_speed, deltaTime))
            {
                m_position = m_doorPos;
                m_stage = MovementStage::Exited;
            }
            break;
        }
        
        default:
            break;
    }
}

bool Person::moveToward(float& current, float target, float speed, float dt)
{
    float diff = target - current;
    
    if (std::abs(diff) < EPSILON)
    {
        current = target;
        return true;  // Reached target
    }
    
    float step = speed * dt;
    if (std::abs(diff) < step)
    {
        current = target;
        return true;
    }
    
    current += (diff > 0 ? step : -step);
    return false;
}

void Person::startExiting()
{
    if (m_stage == MovementStage::Seated)
    {
        m_mode = TravelMode::Exiting;
        m_stage = MovementStage::ToSeatX;  // Start exit sequence
    }
}
