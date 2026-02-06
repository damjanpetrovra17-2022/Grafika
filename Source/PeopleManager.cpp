#include "../Header/PeopleManager.h"
#include "../Header/SeatGrid.h"
#include "../Header/Seat.h"
#include "../Header/DebugCube.h"
#include "../Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <random>
#include <ctime>

PeopleManager::PeopleManager()
{
}

PeopleManager::~PeopleManager()
{
}

void PeopleManager::spawnPeople(int count, SeatGrid& grid, const glm::vec3& doorPos)
{
    if (count <= 0)
        return;
    
    // Collect all occupied seats (Reserved or Purchased)
    std::vector<Seat*> occupiedSeats;
    
    for (int row = 0; row < SeatGrid::ROWS; ++row)
    {
        for (int col = 0; col < SeatGrid::COLS; ++col)
        {
            Seat* seat = grid.getSeat(row, col);
            if (seat && (seat->state == SeatState::Reserved || seat->state == SeatState::Purchased))
            {
                occupiedSeats.push_back(seat);
            }
        }
    }
    
    if (occupiedSeats.empty())
        return;
    
    // Clamp count to available seats
    count = std::min(count, (int)occupiedSeats.size());
    
    // Shuffle to get random unique seats
    static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::shuffle(occupiedSeats.begin(), occupiedSeats.end(), rng);
    
    // Spawn people for first K seats
    for (int i = 0; i < count; ++i)
    {
        Seat* targetSeat = occupiedSeats[i];
        glm::vec3 color = generateRandomColor();
        
        m_people.push_back(std::make_unique<Person>(doorPos, targetSeat, color));
    }
}

void PeopleManager::spawnPeopleRandom(SeatGrid& grid, const glm::vec3& doorPos, int minCount, int maxCount)
{
    // Count occupied seats
    int occupiedCount = 0;
    for (int row = 0; row < SeatGrid::ROWS; ++row)
    {
        for (int col = 0; col < SeatGrid::COLS; ++col)
        {
            Seat* seat = grid.getSeat(row, col);
            if (seat && (seat->state == SeatState::Reserved || seat->state == SeatState::Purchased))
            {
                occupiedCount++;
            }
        }
    }
    
    if (occupiedCount == 0)
        return;
    
    // Random count between min and occupied
    static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(minCount, std::min(maxCount, occupiedCount));
    int count = dist(rng);
    
    spawnPeople(count, grid, doorPos);
}

void PeopleManager::clear()
{
    m_people.clear();
}

void PeopleManager::update(float deltaTime)
{
    for (auto& person : m_people)
    {
        person->update(deltaTime);
    }
}

void PeopleManager::draw(Shader& phongShader, const glm::mat4& view, const glm::mat4& projection, 
                         const glm::vec3& viewPos, DebugCube& cubeMesh)
{
    phongShader.use();
    phongShader.setMat4("view", view);
    phongShader.setMat4("projection", projection);
    phongShader.setVec3("viewPos", viewPos);
    
    // Set lighting (room light)
    phongShader.setVec3("lightPos", glm::vec3(0.0f, 4.0f, 0.0f));
    phongShader.setVec3("lightColor", glm::vec3(1.0f, 0.95f, 0.85f));
    phongShader.setFloat("lightIntensity", 5.0f);
    glUniform1i(glGetUniformLocation(phongShader.ID, "lightEnabled"), 1);
    
    // Draw each person as a cuboid
    for (const auto& person : m_people)
    {
        glm::vec3 pos = person->getPosition();
        glm::vec3 color = person->getColor();
        
        // Build model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        model = glm::scale(model, glm::vec3(PERSON_WIDTH, PERSON_HEIGHT, PERSON_DEPTH));
        
        phongShader.setMat4("model", model);
        phongShader.setVec3("uBaseColor", color);
        
        cubeMesh.draw();
    }
}

bool PeopleManager::allSeated() const
{
    if (m_people.empty())
        return false;
    
    for (const auto& person : m_people)
    {
        if (!person->isSeated())
            return false;
    }
    
    return true;
}

bool PeopleManager::allExited() const
{
    if (m_people.empty())
        return true;  // No people means all exited
    
    for (const auto& person : m_people)
    {
        if (!person->isExited())
            return false;
    }
    
    return true;
}

void PeopleManager::startExiting()
{
    for (auto& person : m_people)
    {
        person->startExiting();
    }
}

glm::vec3 PeopleManager::generateRandomColor() const
{
    static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_real_distribution<float> dist(0.3f, 0.9f);
    
    return glm::vec3(dist(rng), dist(rng), dist(rng));
}
