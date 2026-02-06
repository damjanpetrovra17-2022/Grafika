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
    // No animation yet - static scene
    (void)deltaTime;
}

void Scene::createLights()
{
    // Room light: slightly below ceiling, warm color, enabled by default
    m_roomLight = Light(
        glm::vec3(0.0f, 4.0f, 0.0f),      // Position below ceiling (was 4.5)
        glm::vec3(1.0f, 0.95f, 0.85f),    // Warm white color
        5.0f,                              // Reduced intensity (was 10.0)
        true                               // Enabled by default
    );
    
    // Screen light: near screen, white color, disabled by default
    m_screenLight = Light(
        glm::vec3(0.0f, 3.0f, -8.5f),     // Position at screen
        glm::vec3(1.0f, 1.0f, 1.0f),      // Pure white
        8.0f,                              // Intensity
        false                              // Disabled by default
    );
}

void Scene::draw(const glm::mat4& view, const glm::mat4& projection, 
                 Shader* phongShader, Shader* basicShader, const glm::vec3& viewPos)
{
    if (!m_cubeMesh || !phongShader || !basicShader)
        return;
    
    // Use the active light (room light or screen light, or both)
    // For simplicity, we'll use roomLight as primary, but could blend both
    Light* activeLight = &m_roomLight;
    if (m_screenLight.enabled && !m_roomLight.enabled)
        activeLight = &m_screenLight;
    // If both enabled, use room light (could be extended to support multiple lights)
    
    // Draw all scene objects (hall geometry only - Screen component handles screen)
    for (size_t i = 0; i < m_objects.size(); ++i)
    {
        const auto& obj = m_objects[i];
        
        // All objects use Phong lighting (no isScreen objects anymore)
        phongShader->use();
        phongShader->setMat4("model", obj.modelMatrix());
        phongShader->setMat4("view", view);
        phongShader->setMat4("projection", projection);
        
        // Material
        phongShader->setVec3("uBaseColor", obj.color);
        
        // Light properties
        phongShader->setVec3("lightPos", activeLight->position);
        phongShader->setVec3("lightColor", activeLight->color);
        phongShader->setFloat("lightIntensity", activeLight->intensity);
        
        // Use a bool to enable/disable lighting
        int lightEnabledInt = activeLight->enabled ? 1 : 0;
        glUniform1i(glGetUniformLocation(phongShader->ID, "lightEnabled"), lightEnabledInt);
        
        // View position (camera)
        phongShader->setVec3("viewPos", viewPos);
        
        m_cubeMesh->draw();
    }
}

void Scene::createHallGeometry()
{
    // Cinema hall dimensions (matching AABB bounds from Phase 2)
    // X: -9 to +9 (18m wide)
    // Y: 0.5 to 5.0 (4.5m tall)
    // Z: -9 to +9 (18m deep)
    
    const float hallWidth = 18.0f;
    const float hallDepth = 18.0f;
    const float hallHeight = 4.5f;
    const float wallThickness = 0.2f;
    
    const float floorY = 0.5f;
    const float ceilingY = 5.0f;
    
    // Colors
    const glm::vec3 floorColor(0.25f, 0.25f, 0.3f);      // Medium gray-blue
    const glm::vec3 wallColor(0.2f, 0.2f, 0.22f);        // Darker gray
    const glm::vec3 ceilingColor(0.3f, 0.3f, 0.32f);     // Slightly lighter gray
    const glm::vec3 screenColor(1.0f, 1.0f, 1.0f);       // Pure white (IMPORTANT)
    const glm::vec3 doorColor(0.4f, 0.25f, 0.15f);       // Brown-ish for door
    const glm::vec3 platformColor(0.3f, 0.25f, 0.2f);    // Dark brown for platform
    const glm::vec3 stairsColor(0.28f, 0.24f, 0.22f);    // Slightly darker for stairs
    
    // Clear any existing objects
    m_objects.clear();
    
    // 1. FLOOR
    // Large flat cube at bottom
    m_floorIndex = (int)m_objects.size();
    m_objects.push_back(SceneObject(
        glm::vec3(0.0f, floorY - 0.05f, 0.0f),  // Position (slightly below floor level)
        glm::vec3(hallWidth, 0.1f, hallDepth),   // Scale (thin horizontal slab)
        floorColor
    ));
    
    // 2. CEILING
    // Large flat cube at top
    m_ceilingIndex = (int)m_objects.size();
    m_objects.push_back(SceneObject(
        glm::vec3(0.0f, ceilingY + 0.05f, 0.0f), // Position (slightly above ceiling level)
        glm::vec3(hallWidth, 0.1f, hallDepth),    // Scale (thin horizontal slab)
        ceilingColor
    ));
    
    // 3. FRONT WALL (at -Z, holds screen)
    // Wall with screen cutout (simplified: just full wall, screen rendered on top)
    const float frontZ = -9.0f;
    m_frontWallIndex = (int)m_objects.size();
    m_objects.push_back(SceneObject(
        glm::vec3(0.0f, (floorY + ceilingY) / 2.0f, frontZ), // Center of wall
        glm::vec3(hallWidth, hallHeight, wallThickness),     // Vertical wall
        wallColor
    ));
    
    // 4. BACK WALL (at +Z, no door - door moved to left wall)
    const float backZ = 9.0f;
    m_backWallIndex = (int)m_objects.size();
    m_objects.push_back(SceneObject(
        glm::vec3(0.0f, (floorY + ceilingY) / 2.0f, backZ), // Center of wall
        glm::vec3(hallWidth, hallHeight, wallThickness),    // Vertical wall
        wallColor
    ));
    
    // 5. LEFT WALL (at -X, now holds door)
    const float leftX = -9.0f;
    m_leftWallIndex = (int)m_objects.size();
    m_objects.push_back(SceneObject(
        glm::vec3(leftX, (floorY + ceilingY) / 2.0f, 0.0f), // Center of wall
        glm::vec3(wallThickness, hallHeight, hallDepth),    // Vertical wall (depth-aligned)
        wallColor
    ));
    
    // 6. RIGHT WALL (at +X)
    const float rightX = 9.0f;
    m_rightWallIndex = (int)m_objects.size();
    m_objects.push_back(SceneObject(
        glm::vec3(rightX, (floorY + ceilingY) / 2.0f, 0.0f), // Center of wall
        glm::vec3(wallThickness, hallHeight, hallDepth),     // Vertical wall (depth-aligned)
        wallColor
    ));
    
    // 7. SCREEN - REMOVED
    // Screen is now handled entirely by Screen component (Phase 9)
    // No duplicate screen object in Scene
    m_screenIndex = -1;  // Mark as not used
    
    // 8. DOOR - REMOVED
    // Door is now handled entirely by Door component (Phase 10)
    // No duplicate door object in Scene
    m_doorIndex = -1;  // Mark as not used
    
    // 9. CENTRAL STAIRS (between left and right seat sections)
    // Stairs connect from FLOOR to platforms
    // Each step increases height to match platform levels
    // NOTE: Platforms are handled by SeatGrid, NOT Scene
    
    m_firstStairIndex = (int)m_objects.size();  // Track first stair
    
    const float stepWidth = 3.0f;        // Width of staircase (wider for easier access)
    const float stepHeight = 0.3f;       // Height of each step (matches rowElevationStep)
    const float stepDepth = 1.2f;        // Depth of each step (matches seatSpacingZ)
    const float stairsStartZ = 1.4f;     // Start position (in front of first row)
    
    // Create 5 steps - each step is a SOLID BLOCK from floor to its top
    for (int i = 0; i < 5; ++i)
    {
        // This step's top height
        float stepTopY = floorY + (i + 1) * stepHeight;
        
        // Step extends from FLOOR to its top (solid block, not thin slab)
        float thisStepHeight = stepTopY - floorY;
        
        // Step center Y (halfway between floor and top)
        float stepCenterY = floorY + thisStepHeight * 0.5f;
        
        // Step Z position
        float stepZ = stairsStartZ + i * stepDepth;
        
        m_objects.push_back(SceneObject(
            glm::vec3(0.0f, stepCenterY, stepZ),
            glm::vec3(stepWidth, thisStepHeight, stepDepth),
            stairsColor
        ));
    }
}




std::vector<AABB> Scene::getCollidableBounds() const
{
    std::vector<AABB> bounds;
    
    // Only add stairs (platforms are handled by SeatGrid)
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






