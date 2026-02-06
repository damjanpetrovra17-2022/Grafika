#include "../Header/Door.h"
#include "../Header/DebugCube.h"
#include "../Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <algorithm>

Door::Door()
    : m_isOpen(false)
    , m_position(-8.9f, 1.75f, -5.0f)     // Center of door
    , m_hingePosition(-8.9f, 1.75f, -4.0f) // Hinge at one edge (Z - half width)
    , m_size(0.1f, 2.5f, 2.0f)            // Width, Height, Depth
    , m_color(0.5f, 0.3f, 0.2f)           // Wood color
    , m_currentAngle(0.0f)
    , m_targetAngle(0.0f)
    , m_rotationSpeed(90.0f)              // 90 degrees per second
    , m_cubeMesh(nullptr)
{
}

Door::~Door()
{
}

void Door::init(DebugCube* cubeMesh)
{
    m_cubeMesh = cubeMesh;
}

void Door::update(float deltaTime)
{
    // Smooth animation toward target angle
    if (m_currentAngle != m_targetAngle)
    {
        float diff = m_targetAngle - m_currentAngle;
        float step = m_rotationSpeed * deltaTime;
        
        if (std::abs(diff) < step)
        {
            m_currentAngle = m_targetAngle;
        }
        else
        {
            m_currentAngle += (diff > 0 ? step : -step);
        }
    }
}

void Door::open()
{
    m_isOpen = true;
    m_targetAngle = -90.0f;  // Rotate -90 degrees to open inward (into cinema)
}

void Door::close()
{
    m_isOpen = false;
    m_targetAngle = 0.0f;   // Return to 0 degrees
}

void Door::draw(Shader* shader, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos)
{
    if (!m_cubeMesh || !shader)
        return;
    
    shader->use();
    
    // Build model matrix with rotation around hinge
    glm::mat4 model = glm::mat4(1.0f);
    
    // Move to hinge position
    model = glm::translate(model, m_hingePosition);
    
    // Rotate around Y axis (vertical hinge)
    model = glm::rotate(model, glm::radians(m_currentAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    
    // Offset from hinge to door center (half the door width)
    glm::vec3 offsetFromHinge(0.0f, 0.0f, -m_size.z * 0.5f);
    model = glm::translate(model, offsetFromHinge);
    
    // Scale to door size
    model = glm::scale(model, m_size);
    
    shader->setMat4("model", model);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    
    // Door color
    shader->setVec3("uBaseColor", m_color);
    
    // Set light properties
    shader->setVec3("lightPos", glm::vec3(0.0f, 4.0f, 0.0f));
    shader->setVec3("lightColor", glm::vec3(1.0f, 0.95f, 0.85f));
    shader->setFloat("lightIntensity", 5.0f);
    glUniform1i(glGetUniformLocation(shader->ID, "lightEnabled"), 1);
    shader->setVec3("viewPos", viewPos);
    
    m_cubeMesh->draw();
}
