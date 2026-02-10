#pragma once

#include "Person.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

class SeatGrid;
class Shader;
class DebugCube;
class HumanMesh;

class PeopleManager
{
public:
    PeopleManager();
    ~PeopleManager();
    
    
    void setHumanMesh(HumanMesh* mesh);
    void setHumanShader(Shader* shader);
    
    
    void spawnPeople(int count, SeatGrid& grid, const glm::vec3& doorPos);
    
    
    void spawnPeopleRandom(SeatGrid& grid, const glm::vec3& doorPos, int minCount, int maxCount);
    
    
    void clear();
    
    
    void update(float deltaTime);
    
    
    void draw(Shader& phongShader, const glm::mat4& view, const glm::mat4& projection, 
              const glm::vec3& viewPos, DebugCube& cubeMesh);
    
    
    bool allSeated() const;
    bool allExited() const;
    int getPeopleCount() const { return (int)m_people.size(); }
    
    
    void startExiting();
    
private:
    std::vector<std::unique_ptr<Person>> m_people;
    
    
    
    struct SpawnRequest
    {
        Seat* targetSeat;
        glm::vec3 color;
        int textureIndex;
    };
    std::vector<SpawnRequest> m_spawnQueue;
    glm::vec3 m_doorPos;
    float m_spawnTimer;
    static constexpr float SPAWN_INTERVAL = 1.0f;  
    
    HumanMesh* m_humanMesh;    
    Shader*    m_humanShader;  
    
    
    static constexpr float PERSON_WIDTH = 1.1f;
    static constexpr float PERSON_HEIGHT = 1.2f;
    static constexpr float PERSON_DEPTH = 1.1f;
    
    
    glm::vec3 generateRandomColor() const;
};
