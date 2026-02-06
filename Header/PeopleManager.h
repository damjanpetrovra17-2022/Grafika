#pragma once

#include "Person.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

// Forward declarations
class SeatGrid;
class Shader;
class DebugCube;

class PeopleManager
{
public:
    PeopleManager();
    ~PeopleManager();
    
    // Spawn people at door targeting occupied seats
    void spawnPeople(int count, SeatGrid& grid, const glm::vec3& doorPos);
    
    // Spawn random number of people (for debug)
    void spawnPeopleRandom(SeatGrid& grid, const glm::vec3& doorPos, int minCount, int maxCount);
    
    // Clear all people
    void clear();
    
    // Update all people movement
    void update(float deltaTime);
    
    // Draw all people
    void draw(Shader& phongShader, const glm::mat4& view, const glm::mat4& projection, 
              const glm::vec3& viewPos, DebugCube& cubeMesh);
    
    // Status queries
    bool allSeated() const;
    bool allExited() const;
    int getPeopleCount() const { return (int)m_people.size(); }
    
    // For Phase 10 - command all to exit
    void startExiting();
    
private:
    std::vector<std::unique_ptr<Person>> m_people;
    
    // Person rendering dimensions
    static constexpr float PERSON_WIDTH = 0.4f;
    static constexpr float PERSON_HEIGHT = 1.7f;
    static constexpr float PERSON_DEPTH = 0.4f;
    
    // Random color generation for visual differentiation
    glm::vec3 generateRandomColor() const;
};
