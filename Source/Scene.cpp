#include "../Header/Scene.h"
#include "../Header/DebugCube.h"
#include "../Header/Camera.h"
#include "../Header/Light.h"
#include "../Shader.h"

Scene::Scene()
    : m_cubeMesh(nullptr)
    , m_floorIndex(-1)
    , m_ceilingIndex(-1)
    , m_frontWallIndex(-1)
    , m_backWallIndex(-1)
    , m_leftWallIndex(-1)
    , m_rightWallIndex(-1)
    , m_screenIndex(-1)
    , m_doorIndex(-1)
    , m_firstPlatformIndex(-1)
    , m_firstStairIndex(-1)
{
}

Scene::~Scene()
{
}

void Scene::init(DebugCube* cubeMesh)
{
    m_cubeMesh = cubeMesh;
    createHallGeometry();
    createLights();
}

void Scene::update(float deltaTime)
{
    
    (void)deltaTime;
}

void Scene::createLights()
{
    
    m_roomLight = Light(
        glm::vec3(0.0f, 4.0f, 0.0f),      
        glm::vec3(1.0f, 0.95f, 0.85f),    
        5.0f,                              
        true                               
    );
    
    
    m_screenLight = Light(
        glm::vec3(0.0f, 3.0f, -8.5f),     
        glm::vec3(1.0f, 1.0f, 1.0f),      
        8.0f,                              
        false                              
    );
}

void Scene::draw(const glm::mat4& view, const glm::mat4& projection, 
                 Shader* phongShader, Shader* basicShader, const glm::vec3& viewPos)
{
    if (!m_cubeMesh || !phongShader || !basicShader)
        return;
    
    
    
    Light* activeLight = &m_roomLight;
    if (m_screenLight.enabled && !m_roomLight.enabled)
        activeLight = &m_screenLight;
    
    
    
    for (size_t i = 0; i < m_objects.size(); ++i)
    {
        const auto& obj = m_objects[i];
        
        
        phongShader->use();
        phongShader->setMat4("model", obj.modelMatrix());
        phongShader->setMat4("view", view);
        phongShader->setMat4("projection", projection);
        
        
        phongShader->setVec3("uBaseColor", obj.color);
        
        
        phongShader->setVec3("lightPos", activeLight->position);
        phongShader->setVec3("lightColor", activeLight->color);
        phongShader->setFloat("lightIntensity", activeLight->intensity);
        
        
        int lightEnabledInt = activeLight->enabled ? 1 : 0;
        glUniform1i(glGetUniformLocation(phongShader->ID, "lightEnabled"), lightEnabledInt);
        
        
        phongShader->setVec3("viewPos", viewPos);
        
        m_cubeMesh->draw();
    }
}

void Scene::createHallGeometry()
{
    
    
    
    
    
    const float hallWidth = 18.0f;
    const float hallDepth = 18.0f;
    const float hallHeight = 4.5f;
    const float wallThickness = 0.2f;
    
    const float floorY = 0.5f;
    const float ceilingY = 5.0f;
    
    
    const glm::vec3 floorColor(0.25f, 0.25f, 0.3f);      
    const glm::vec3 wallColor(0.2f, 0.2f, 0.22f);        
    const glm::vec3 ceilingColor(0.3f, 0.3f, 0.32f);     
    const glm::vec3 screenColor(1.0f, 1.0f, 1.0f);       
    const glm::vec3 doorColor(0.4f, 0.25f, 0.15f);       
    const glm::vec3 platformColor(0.3f, 0.25f, 0.2f);    
    const glm::vec3 stairsColor(0.28f, 0.24f, 0.22f);    
    
    
    m_objects.clear();
    
    
    
    m_floorIndex = (int)m_objects.size();
    m_objects.push_back(SceneObject(
        glm::vec3(0.0f, floorY - 0.05f, 0.0f),  
        glm::vec3(hallWidth, 0.1f, hallDepth),   
        floorColor
    ));
    
    
    
    m_ceilingIndex = (int)m_objects.size();
    m_objects.push_back(SceneObject(
        glm::vec3(0.0f, ceilingY + 0.05f, 0.0f), 
        glm::vec3(hallWidth, 0.1f, hallDepth),    
        ceilingColor
    ));
    
    
    
    const float frontZ = -9.0f;
    m_frontWallIndex = (int)m_objects.size();
    m_objects.push_back(SceneObject(
        glm::vec3(0.0f, (floorY + ceilingY) / 2.0f, frontZ), 
        glm::vec3(hallWidth, hallHeight, wallThickness),     
        wallColor
    ));
    
    
    const float backZ = 9.0f;
    m_backWallIndex = (int)m_objects.size();
    m_objects.push_back(SceneObject(
        glm::vec3(0.0f, (floorY + ceilingY) / 2.0f, backZ), 
        glm::vec3(hallWidth, hallHeight, wallThickness),    
        wallColor
    ));
    
    
    const float leftX = -9.0f;
    m_leftWallIndex = (int)m_objects.size();
    m_objects.push_back(SceneObject(
        glm::vec3(leftX, (floorY + ceilingY) / 2.0f, 0.0f), 
        glm::vec3(wallThickness, hallHeight, hallDepth),    
        wallColor
    ));
    
    
    const float rightX = 9.0f;
    m_rightWallIndex = (int)m_objects.size();
    m_objects.push_back(SceneObject(
        glm::vec3(rightX, (floorY + ceilingY) / 2.0f, 0.0f), 
        glm::vec3(wallThickness, hallHeight, hallDepth),     
        wallColor
    ));
    
    
    
    
    m_screenIndex = -1;  
    
    
    
    
    m_doorIndex = -1;  
    
    
    
    
    
    
    m_firstStairIndex = (int)m_objects.size();  
    
    const float stepWidth = 2.9f;        
    const float stepHeight = 0.3f;       
    const float stepDepth = 1.2f;        
    const float stairsStartZ = 1.2f;    
    
    
    
    
    const float leftStairsX = -8.0f;  
    
    for (int i = 0; i < 5; ++i)
    {
        
        float stepTopY = floorY + (i + 1) * stepHeight;
        
        
        float thisStepHeight = stepTopY - floorY;
        
        
        float stepCenterY = floorY + thisStepHeight * 0.5f;
        
        
        float stepZ = stairsStartZ + i * stepDepth;
        
        m_objects.push_back(SceneObject(
            glm::vec3(leftStairsX, stepCenterY, stepZ),
            glm::vec3(stepWidth, thisStepHeight, stepDepth),
            stairsColor
        ));
    }
}

std::vector<AABB> Scene::getCollidableBounds() const
{
    std::vector<AABB> bounds;
    
    
    
    if (m_firstStairIndex >= 0)
    {
        for (int i = 0; i < 5; ++i)  
        {
            int idx = m_firstStairIndex + i;
            if (idx < (int)m_objects.size())
            {
                const SceneObject& obj = m_objects[idx];
                glm::vec3 halfExtents = obj.scale * 0.5f;
                bounds.push_back(AABB(
                    obj.position - halfExtents,
                    obj.position + halfExtents
                ));
            }
        }
    }
    
    return bounds;
}

