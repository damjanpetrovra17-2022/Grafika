#pragma once

#include <glm/glm.hpp>

struct Seat;

enum class MovementStage
{
    ToRowDepth,       
    ClimbToRowHeight, 
    ToSeatX,          
    Seated,           
    Exited            
};

enum class TravelMode
{
    Entering,  
    Exiting    
};

class Person
{
public:
    Person();
    Person(const glm::vec3& doorPos, Seat* targetSeat, const glm::vec3& baseColor, int textureIndex = 0);
    
    void update(float deltaTime);
    
    
    const glm::vec3& getPosition() const { return m_position; }
    const glm::vec3& getColor() const { return m_baseColor; }
    float getRotationY() const { return m_rotationY; }
    int getTextureIndex() const { return m_textureIndex; }
    MovementStage getStage() const { return m_stage; }
    bool isSeated() const { return m_stage == MovementStage::Seated; }
    bool isExited() const { return m_stage == MovementStage::Exited; }
    
    
    void startExiting();
    
private:
glm::vec3 m_position;      
glm::vec3 m_doorPos;       
Seat* m_targetSeat;        
    
MovementStage m_stage;
TravelMode m_mode;
    
float m_speed;
float m_rotationY;
glm::vec3 m_baseColor;
int m_textureIndex;
    
    
    static constexpr float EPSILON = 0.05f;
    static constexpr float LEFT_AISLE_X = -8.0f;   
    static constexpr float RIGHT_AISLE_X = 7.0f;   
    
void updateEntering(float deltaTime);
void updateExiting(float deltaTime);
    
bool moveToward(float& current, float target, float speed, float dt);
    

float getAisleX() const;
};
