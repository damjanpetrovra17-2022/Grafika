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
    , m_rotationY(0.0f)
    , m_baseColor(1.0f)
    , m_textureIndex(0)
{
}

Person::Person(const glm::vec3& doorPos, Seat* targetSeat, const glm::vec3& baseColor, int textureIndex)
    : m_position(doorPos)
    , m_doorPos(doorPos)
    , m_targetSeat(targetSeat)
    , m_stage(MovementStage::ToRowDepth)
    , m_mode(TravelMode::Entering)
    , m_speed(2.0f)
    , m_rotationY(0.0f)
    , m_baseColor(baseColor)
    , m_textureIndex(textureIndex)
{
    
    m_position.y = 0.5f + 0.6f;
}

float Person::getAisleX() const
{
    
    return LEFT_AISLE_X;
}

void Person::update(float deltaTime)
{
    if (m_stage == MovementStage::Seated || m_stage == MovementStage::Exited)
        return;  
    
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
    const float aisleX = getAisleX();  
    
    switch (m_stage)
    {
        case MovementStage::ToRowDepth:
        {
            
            
            m_position.x = aisleX;
            
            
            if (seatPos.z > m_position.z)
                m_rotationY = 0.0f;  
            else
                m_rotationY = 3.14159f;  
            
            
            
            
            const float rowSpacing = 1.2f;  
            const float rowElevationStep = 0.3f;  
            const float originZ = 2.0f;  
            const float groundFloorY = 0.5f;  
            
            
            float targetRow = (seatPos.z - originZ) / rowSpacing;
            float currentRow = (m_position.z - originZ) / rowSpacing;
            
            
            if (currentRow < 0.0f) currentRow = 0.0f;
            if (currentRow > targetRow) currentRow = targetRow;
            
            
            float groundY = groundFloorY + currentRow * rowElevationStep + 0.6f;  
            
            
            m_position.y = groundY;
            
            if (moveToward(m_position.z, seatPos.z, m_speed, deltaTime))
            {
                m_stage = MovementStage::ClimbToRowHeight;
            }
            break;
        }
        
        case MovementStage::ClimbToRowHeight:
        {
            
            
            if (moveToward(m_position.y, seatPos.y, m_speed, deltaTime))
            {
                m_stage = MovementStage::ToSeatX;
            }
            break;
        }
        
        case MovementStage::ToSeatX:
        {
            
            
            
            
            if (seatPos.x > m_position.x)
                m_rotationY = 1.5708f;  
            else
                m_rotationY = -1.5708f;  
            
            if (moveToward(m_position.x, seatPos.x, m_speed, deltaTime))
            {
                
                const float personHeight = 1.2f;
                m_position = seatPos + glm::vec3(0.0f, personHeight * 0.5f, 0.0f);
                m_rotationY = 3.14159f;  
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
    
    const float aisleX = getAisleX();  
    
    switch (m_stage)
    {
        case MovementStage::Seated:
        {
            
            m_stage = MovementStage::ToSeatX;  
            break;
        }
        
        case MovementStage::ToSeatX:
        {
            
            
            if (aisleX > m_position.x)
                m_rotationY = 1.5708f;  
            else
                m_rotationY = -1.5708f;  
            
            if (moveToward(m_position.x, aisleX, m_speed, deltaTime))
            {
                m_stage = MovementStage::ClimbToRowHeight;
            }
            break;
        }
        
        case MovementStage::ClimbToRowHeight:
        {
            
            m_stage = MovementStage::ToRowDepth;
            break;
        }
        
        case MovementStage::ToRowDepth:
        {
            
            m_position.x = aisleX;
            
            
            if (m_doorPos.z > m_position.z)
                m_rotationY = 0.0f;  
            else
                m_rotationY = 3.14159f;  
            
            
            const float rowSpacing = 1.2f;
            const float rowElevationStep = 0.3f;
            const float originZ = 2.0f;
            const float groundFloorY = 0.5f;
            
            float currentRow = (m_position.z - originZ) / rowSpacing;
            if (currentRow < 0.0f) currentRow = 0.0f;
            
            
            float groundY = groundFloorY + currentRow * rowElevationStep + 0.6f;
            
            
            m_position.y = groundY;
            
            if (moveToward(m_position.z, m_doorPos.z, m_speed, deltaTime))
            {
                m_position.x = m_doorPos.x;
                m_position.y = groundFloorY + 0.6f;  
                m_position.z = m_doorPos.z;
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
        return true;  
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
        m_stage = MovementStage::ToSeatX;  
    }
}
