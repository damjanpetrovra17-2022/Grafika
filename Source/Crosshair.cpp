#include "../Header/Crosshair.h"
#include "../Shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Crosshair::Crosshair()
    : m_VAO(0)
    , m_VBO(0)
    , m_initialized(false)
{
}

Crosshair::~Crosshair()
{
    cleanup();
}

void Crosshair::init()
{
    if (m_initialized) return;
    
    // Subtle crosshair: small cross in center
    // Smaller and less intrusive than before
    float size = 0.008f;  // Very small (0.8% of screen - was 2%)
    
    float vertices[] = {
        // Horizontal line
        -size, 0.0f,  0.0f,
         size, 0.0f,  0.0f,
        
        // Vertical line
         0.0f, -size, 0.0f,
         0.0f,  size, 0.0f
    };
    
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
    
    m_initialized = true;
}

void Crosshair::draw(Shader* shader, int screenWidth, int screenHeight)
{
    if (!m_initialized || !shader) return;
    
    shader->use();
    
    // Identity matrices (crosshair is in NDC space already)
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    
    shader->setMat4("model", model);
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);
    
    // Subtle gray color (barely visible but enough to see)
    shader->setVec3("objectColor", glm::vec3(0.5f, 0.5f, 0.5f));
    
    // Disable depth test for crosshair (always on top)
    glDisable(GL_DEPTH_TEST);
    
    // Draw lines
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_LINES, 0, 4);  // 4 vertices (2 lines)
    glBindVertexArray(0);
    
    // Re-enable depth test
    glEnable(GL_DEPTH_TEST);
}

void Crosshair::cleanup()
{
    if (m_initialized)
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        m_VAO = 0;
        m_VBO = 0;
        m_initialized = false;
    }
}
