#pragma once

#include "Light.h"
#include "AABB.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>

class Shader;
class DebugCube;
class Camera;

struct SceneObject
{
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 color;
    bool isScreen;  
    
    SceneObject()
        : position(0.0f)
        , scale(1.0f)
        , color(1.0f)
        , isScreen(false)
    {
    }
    
    SceneObject(const glm::vec3& pos, const glm::vec3& scl, const glm::vec3& col, bool screen = false)
        : position(pos)
        , scale(scl)
        , color(col)
        , isScreen(screen)
    {
    }
    
    glm::mat4 modelMatrix() const
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, scale);
        return model;
    }
};

class Scene
{
public:
    Scene();
    ~Scene();
    
    void init(DebugCube* cubeMesh);
    void update(float deltaTime);
    void draw(const glm::mat4& view, const glm::mat4& projection, 
              Shader* phongShader, Shader* basicShader, const glm::vec3& viewPos);
    
    
    Light& getRoomLight() { return m_roomLight; }
    Light& getScreenLight() { return m_screenLight; }
    
    
    std::vector<AABB> getCollidableBounds() const;
    
private:
    void createHallGeometry();
    void createLights();
    
    DebugCube* m_cubeMesh;  
    std::vector<SceneObject> m_objects;
    
    
    Light m_roomLight;
    Light m_screenLight;
    
    
    int m_floorIndex;
    int m_ceilingIndex;
    int m_frontWallIndex;
    int m_backWallIndex;
    int m_leftWallIndex;
    int m_rightWallIndex;
    int m_screenIndex;
    int m_doorIndex;
    
    
    int m_firstPlatformIndex;  
    int m_firstStairIndex;     
};
