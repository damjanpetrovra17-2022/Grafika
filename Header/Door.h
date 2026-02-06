#pragma once

#include <glm/glm.hpp>

class Shader;
class DebugCube;

class Door
{
public:
    Door();
    ~Door();
    
    void init(DebugCube* cubeMesh);
    void update(float deltaTime);
    
    void open();
    void close();
    bool isOpen() const { return m_isOpen; }
    bool isAnimating() const { return m_currentAngle != m_targetAngle; }
    
    void draw(Shader* shader, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos);
    
    const glm::vec3& getPosition() const { return m_position; }
    
private:
    bool m_isOpen;
    glm::vec3 m_position;
    glm::vec3 m_hingePosition;  // Point where door rotates
    glm::vec3 m_size;
    glm::vec3 m_color;
    
    // Animation
    float m_currentAngle;  // Current rotation angle
    float m_targetAngle;   // Target rotation angle
    float m_rotationSpeed; // Degrees per second
    
    DebugCube* m_cubeMesh;
};
