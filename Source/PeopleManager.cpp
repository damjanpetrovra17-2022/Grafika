#include "../Header/PeopleManager.h"
#include "../Header/SeatGrid.h"
#include "../Header/Seat.h"
#include "../Header/DebugCube.h"
#include "../Header/HumanMesh.h"
#include "../Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <random>
#include <ctime>

PeopleManager::PeopleManager()
    : m_humanMesh(nullptr)
    , m_humanShader(nullptr)
    , m_doorPos(0.0f)
    , m_spawnTimer(0.0f)
{
}

void PeopleManager::setHumanMesh(HumanMesh* mesh)
{
    m_humanMesh = mesh;
}

void PeopleManager::setHumanShader(Shader* shader)
{
    m_humanShader = shader;
}

PeopleManager::~PeopleManager()
{
}

void PeopleManager::spawnPeople(int count, SeatGrid& grid, const glm::vec3& doorPos)
{
    if (count <= 0)
        return;
    
    
    m_doorPos = doorPos;
    
    
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
    
    
    count = std::min(count, (int)occupiedSeats.size());
    
    
    static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::shuffle(occupiedSeats.begin(), occupiedSeats.end(), rng);
    
    
    
    m_spawnQueue.clear();
    
    int textureCount = (m_humanMesh && m_humanMesh->getTextureCount() > 0) ? m_humanMesh->getTextureCount() : 1;
    std::uniform_int_distribution<int> textureDist(0, textureCount - 1);
    
    for (int i = 0; i < count; ++i)
    {
        Seat* targetSeat = occupiedSeats[i];
        glm::vec3 color = generateRandomColor();
        int textureIndex = textureDist(rng);
        
        SpawnRequest req;
        req.targetSeat = targetSeat;
        req.color = color;
        req.textureIndex = textureIndex;
        m_spawnQueue.push_back(req);
    }
    
    
    m_spawnTimer = 0.0f;
}

void PeopleManager::spawnPeopleRandom(SeatGrid& grid, const glm::vec3& doorPos, int minCount, int maxCount)
{
    
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
    
    
    static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(minCount, std::min(maxCount, occupiedCount));
    int count = dist(rng);
    
    spawnPeople(count, grid, doorPos);
}

void PeopleManager::clear()
{
    m_people.clear();
    m_spawnQueue.clear();
    m_spawnTimer = 0.0f;
}

void PeopleManager::update(float deltaTime)
{
    
    if (!m_spawnQueue.empty())
    {
        m_spawnTimer += deltaTime;
        
        if (m_spawnTimer >= SPAWN_INTERVAL)
        {
            
            SpawnRequest req = m_spawnQueue.front();
            m_spawnQueue.erase(m_spawnQueue.begin());
            
            m_people.push_back(std::make_unique<Person>(m_doorPos, req.targetSeat, req.color, req.textureIndex));
            
            
            m_spawnTimer = 0.0f;
        }
    }
    
    
    for (auto& person : m_people)
    {
        person->update(deltaTime);
    }
}

void PeopleManager::draw(Shader& phongShader, const glm::mat4& view, const glm::mat4& projection, 
                         const glm::vec3& viewPos, DebugCube& cubeMesh)
{
    if (m_humanMesh && m_humanShader)
    {
        m_humanShader->use();
        m_humanShader->setMat4("view", view);
        m_humanShader->setMat4("projection", projection);
        m_humanShader->setVec3("viewPos", viewPos);
        
        
        m_humanShader->setVec3("lightPos", glm::vec3(0.0f, 4.0f, 0.0f));
        m_humanShader->setVec3("lightColor", glm::vec3(1.0f, 0.95f, 0.85f));
        m_humanShader->setFloat("lightIntensity", 5.0f);
        glUniform1i(glGetUniformLocation(m_humanShader->ID, "lightEnabled"), 1);
        
        
        
        glActiveTexture(GL_TEXTURE0);
        m_humanShader->setInt("uTexture", 0);
        
        for (const auto& person : m_people)
        {
            glm::vec3 pos = person->getPosition();
            float rotY = person->getRotationY();
            int texIndex = person->getTextureIndex();
            
            GLuint texID = m_humanMesh->getTextureID(texIndex);
            glBindTexture(GL_TEXTURE_2D, texID);
            
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pos);
            model = glm::rotate(model, rotY, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(PERSON_WIDTH, PERSON_HEIGHT, PERSON_DEPTH));
            
            m_humanShader->setMat4("model", model);
            
            m_humanMesh->draw();
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        
        phongShader.use();
        phongShader.setMat4("view", view);
        phongShader.setMat4("projection", projection);
        phongShader.setVec3("viewPos", viewPos);
        
        phongShader.setVec3("lightPos", glm::vec3(0.0f, 4.0f, 0.0f));
        phongShader.setVec3("lightColor", glm::vec3(1.0f, 0.95f, 0.85f));
        phongShader.setFloat("lightIntensity", 5.0f);
        glUniform1i(glGetUniformLocation(phongShader.ID, "lightEnabled"), 1);
        
        for (const auto& person : m_people)
        {
            glm::vec3 pos = person->getPosition();
            glm::vec3 color = person->getColor();
            float rotY = person->getRotationY();
            
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pos);
            model = glm::rotate(model, rotY, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(PERSON_WIDTH, PERSON_HEIGHT, PERSON_DEPTH));
            
            phongShader.setMat4("model", model);
            phongShader.setVec3("uBaseColor", color);
            
            cubeMesh.draw();
        }
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
        return true;  
    
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
