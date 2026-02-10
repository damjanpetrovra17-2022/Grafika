#include "../Header/Door.h"
#include "../Header/DebugCube.h"
#include "../Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <algorithm>

Door::Door()
    : m_isOpen(false)
    , m_position(-8.9f, 1.75f, -5.0f)     
    , m_hingePosition(-8.9f, 1.75f, -6.0f) 
    , m_size(0.1f, 2.5f, 2.0f)            
    , m_color(0.5f, 0.3f, 0.2f)           
    , m_currentAngle(0.0f)
    , m_targetAngle(0.0f)
    , m_rotationSpeed(90.0f)              
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
    m_targetAngle = 90.0f;  
}

void Door::close()
{
    m_isOpen = false;
    m_targetAngle = 0.0f;   
}

void Door::draw(Shader* shader, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& viewPos)
{
    if (!m_cubeMesh || !shader)
        return;
    
    shader->use();
    
    
    glm::mat4 model = glm::mat4(1.0f);
    
    
    model = glm::translate(model, m_hingePosition);
    
    
    model = glm::rotate(model, glm::radians(m_currentAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    
    
    glm::vec3 offsetFromHinge(0.0f, 0.0f, m_size.z * 0.5f);
    model = glm::translate(model, offsetFromHinge);
    
    
    model = glm::scale(model, m_size);
    
    shader->setMat4("model", model);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    
    
    shader->setVec3("uBaseColor", m_color);
    
    
    shader->setVec3("lightPos", glm::vec3(0.0f, 4.0f, 0.0f));
    shader->setVec3("lightColor", glm::vec3(1.0f, 0.95f, 0.85f));
    shader->setFloat("lightIntensity", 5.0f);
    glUniform1i(glGetUniformLocation(shader->ID, "lightEnabled"), 1);
    shader->setVec3("viewPos", viewPos);
    
    m_cubeMesh->draw();
}
