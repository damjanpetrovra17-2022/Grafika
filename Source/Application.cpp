#include "../Header/Application.h"
#include "../Header/Log.h"
#include "../Header/AppTime.h"
#include "../Header/Window.h"
#include "../Header/FrameLimiter.h"
#include "../Header/Input.h"
#include "../Header/Camera.h"
#include "../Header/DebugCube.h"
#include "../Header/SeatMesh.h"
#include "../Header/HumanMesh.h"
#include "../Header/Scene.h"
#include "../Header/SeatGrid.h"
#include "../Header/RayPicker.h"
#include "../Header/Crosshair.h"
#include "../Header/PeopleManager.h"
#include "../Header/Screen.h"
#include "../Header/Door.h"
#include "../Header/HUD.h"
#include "../Header/AABB.h"
#include "../Shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Application::Application()
    : m_running(false)
    , m_currentState(AppState::Booking)
    , m_stateTimer(0.0f)
    , m_debugPrintTimer(0.0f)
    , m_depthTestEnabled(true)
    , m_cullingEnabled(false)
    , m_window(nullptr)
    , m_frameLimiter(nullptr)
    , m_camera(nullptr)
    , m_debugCube(nullptr)
    , m_seatMesh(nullptr)
    , m_humanMesh(nullptr)
    , m_basicShader(nullptr)
    , m_phongShader(nullptr)
    , m_humanShader(nullptr)
    , m_scene(nullptr)
    , m_seatGrid(nullptr)
    , m_rayPicker(nullptr)
    , m_crosshair(nullptr)
    , m_peopleManager(nullptr)
    , m_screen(nullptr)
    , m_door(nullptr)
    , m_hud(nullptr)
{
}

Application::~Application()
{
}

bool Application::init()
{
    LOG_INFO("Initializing Application...");
    
    Log::init();
    Time::init();
    
    m_window = std::unique_ptr<Window>(new Window());
    if (!m_window->init())
    {
        LOG_ERROR("Failed to initialize window!");
        return false;
    }
    
    Input::init(m_window->handle());
    
    m_frameLimiter = std::unique_ptr<FrameLimiter>(new FrameLimiter(75.0));
    
    m_camera = std::unique_ptr<Camera>(new Camera(
        glm::vec3(0.0f, 1.7f, 8.0f),
        -90.0f,
        0.0f
    ));
    
    AABB hallBounds(
        glm::vec3(-9.0f, 0.5f, -9.0f),
        glm::vec3(9.0f, 5.0f, 9.0f)
    );
    m_camera->setBounds(hallBounds);
    m_camera->setBoundsPadding(0.3f);
    
    m_debugCube = std::unique_ptr<DebugCube>(new DebugCube());
    m_debugCube->init();
    
    m_seatMesh = std::unique_ptr<SeatMesh>(new SeatMesh());
    if (!m_seatMesh->loadOBJ("Assets/Models/seat.obj"))
    {
        LOG_ERROR("Failed to load seat mesh, falling back to cubes");
        m_seatMesh.reset();
    }
    
    m_basicShader = std::unique_ptr<Shader>(new Shader(
        "Assets/Shaders/basic.vert",
        "Assets/Shaders/basic.frag"
    ));
    
    if (m_basicShader->ID == 0)
    {
        LOG_ERROR("Failed to create basic shader!");
        return false;
    }
    
    m_phongShader = std::unique_ptr<Shader>(new Shader(
        "Assets/Shaders/phong.vert",
        "Assets/Shaders/phong.frag"
    ));
    
    if (m_phongShader->ID == 0)
    {
        LOG_ERROR("Failed to create phong shader!");
        return false;
    }
    
    m_humanShader = std::unique_ptr<Shader>(new Shader(
        "Assets/Shaders/human.vert",
        "Assets/Shaders/human.frag"
    ));
    
    if (m_humanShader->ID == 0)
    {
        LOG_ERROR("Failed to create human shader!");
        m_humanShader.reset();
    }
    
    m_humanMesh = std::unique_ptr<HumanMesh>(new HumanMesh());
    if (!m_humanMesh->loadOBJ("Assets/Models/human1.obj"))
    {
        LOG_ERROR("Failed to load human mesh, falling back to cubes");
        m_humanMesh.reset();
    }
    else if (!m_humanMesh->loadMultipleTextures("Assets/Textures", 10))
    {
        LOG_ERROR("Failed to load human textures, falling back to cubes");
        m_humanMesh.reset();
    }
    
    
    glEnable(GL_DEPTH_TEST);
    m_depthTestEnabled = true;
    
    
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    m_cullingEnabled = false;
    
    LOG_INFO("[RENDER] Initial state: DepthTest=ON Culling=OFF");
    
    m_scene = std::unique_ptr<Scene>(new Scene());
    m_scene->init(m_debugCube.get());
    
    m_seatGrid = std::unique_ptr<SeatGrid>(new SeatGrid());
    glm::vec3 seatOrigin(0.0f, 1.0f, 2.0f);
    m_seatGrid->init(m_debugCube.get(), m_seatMesh.get(), seatOrigin, 1.0f, 1.2f, 0.3f);
    
    std::vector<AABB> platformBounds = m_seatGrid->getPlatformBounds();
    std::vector<AABB> sceneBounds = m_scene->getCollidableBounds();
    std::vector<AABB> allBounds;
    allBounds.insert(allBounds.end(), platformBounds.begin(), platformBounds.end());
    allBounds.insert(allBounds.end(), sceneBounds.begin(), sceneBounds.end());
    m_camera->setAdditionalBounds(allBounds);
    
    m_rayPicker = std::unique_ptr<RayPicker>(new RayPicker());
    
    m_crosshair = std::unique_ptr<Crosshair>(new Crosshair());
    m_crosshair->init();
    
    m_peopleManager = std::unique_ptr<PeopleManager>(new PeopleManager());
    if (m_humanMesh && m_humanShader)
    {
        m_peopleManager->setHumanMesh(m_humanMesh.get());
        m_peopleManager->setHumanShader(m_humanShader.get());
    }
    
    m_screen = std::unique_ptr<Screen>(new Screen());
    m_screen->init();
    
    m_door = std::unique_ptr<Door>(new Door());
    m_door->init(m_debugCube.get());
    
    m_hud = std::unique_ptr<HUD>(new HUD());
    m_hud->init(m_window->width(), m_window->height());
    
    
    enterState(AppState::Booking);
    
    m_running = true;
    LOG_INFO("Application initialized successfully");
    LOG_INFO("Press ENTER to start cinema cycle (after selecting seats)");
    
    return true;
}

void Application::run()
{
    LOG_INFO("Starting main loop...");
    
    while (m_running && !m_window->shouldClose())
    {
        m_frameLimiter->beginFrame();
        m_window->pollEvents();
        Input::update();
        Time::update();
        
        float dt = Time::deltaTime();
        
        
        updateStateMachine(dt);
        
        
        if (m_currentState == AppState::Booking)
        {
            handleSeatPicking();
            handlePurchaseKeys();
        }
        handleEnterKey();
        
        
        handleRenderToggles();
        
        
        m_camera->update(dt);
        
        
        if (m_door)
        {
            m_door->update(dt);
        }
        
        
        if (m_peopleManager)
        {
            m_peopleManager->update(dt);
        }
        
        
        if (m_screen)
        {
            m_screen->update(dt);
        }
        
        
        m_debugPrintTimer += dt;
        if (m_debugPrintTimer >= 1.0f)
        {
            m_debugPrintTimer = 0.0f;
            int occupied = countOccupiedSeats();
            int people = m_peopleManager ? m_peopleManager->getPeopleCount() : 0;
            bool playing = m_screen ? m_screen->isPlaying() : false;
            LOG_INFO("[STATE] " + std::string(stateToString(m_currentState)) + 
                     " | occupied=" + std::to_string(occupied) +
                     " people=" + std::to_string(people) +
                     " playing=" + std::to_string(playing) +
                     " | Depth=" + std::string(m_depthTestEnabled ? "ON" : "OFF") +
                     " Cull=" + std::string(m_cullingEnabled ? "ON" : "OFF"));
        }
        
        
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        float aspect = static_cast<float>(m_window->width()) / static_cast<float>(m_window->height());
        glm::mat4 view = m_camera->viewMatrix();
        glm::mat4 projection = m_camera->projectionMatrix(aspect);
        glm::vec3 viewPos = m_camera->getPosition();
        
        m_scene->update(dt);
        m_scene->draw(view, projection, m_phongShader.get(), m_basicShader.get(), viewPos);
        
        if (m_door)
        {
            m_door->draw(m_phongShader.get(), view, projection, viewPos);
        }
        
        m_seatGrid->draw(m_phongShader.get(), view, projection, viewPos);
        
        if (m_peopleManager)
        {
            m_peopleManager->draw(*m_phongShader, view, projection, viewPos, *m_debugCube);
        }
        
        if (m_screen)
        {
            m_screen->draw(view, projection);
        }
        
        m_crosshair->draw(m_basicShader.get(), m_window->width(), m_window->height());
        
        
        if (m_hud)
        {
            m_hud->draw();
        }
        
        m_window->swapBuffers();
        m_frameLimiter->endFrame();
    }
    
    LOG_INFO("Main loop ended");
}

void Application::updateStateMachine(float deltaTime)
{
    m_stateTimer += deltaTime;
    
    switch (m_currentState)
    {
        case AppState::Booking:
            updateBookingState();
            break;
        case AppState::Entering:
            updateEnteringState();
            break;
        case AppState::Projection:
            updateProjectionState();
            break;
        case AppState::Exiting:
            updateExitingState();
            break;
        case AppState::Reset:
            updateResetState();
            break;
    }
}

void Application::enterState(AppState newState)
{
    LOG_INFO("[STATE] Transition: " + std::string(stateToString(m_currentState)) + 
             " -> " + std::string(stateToString(newState)));
    
    m_currentState = newState;
    m_stateTimer = 0.0f;
    
    setLightingForState(newState);
    
    switch (newState)
    {
        case AppState::Booking:
            if (m_screen) m_screen->stopAndResetToWhite();
            if (m_door) m_door->close();
            break;
            
        case AppState::Entering:
        {
            
            if (m_door) m_door->open();
            LOG_INFO("Door opening... waiting for people to enter");
            break;
        }
        
        case AppState::Projection:
            
            if (m_door) m_door->close();
            LOG_INFO("Door closing... preparing projection");
            break;
            
        case AppState::Exiting:
            
            if (m_door) m_door->open();
            if (m_peopleManager) m_peopleManager->startExiting();
            LOG_INFO("Door opening... people exiting");
            break;
            
        case AppState::Reset:
            
            break;
    }
}

void Application::updateBookingState()
{
    
}

void Application::updateEnteringState()
{
    
    const float DOOR_OPEN_DELAY = 1.5f;
    
    if (m_stateTimer < DOOR_OPEN_DELAY)
    {
        
        return;
    }
    
    
    static bool peopleSpawned = false;
    if (!peopleSpawned && m_stateTimer >= DOOR_OPEN_DELAY)
    {
        int occupied = countOccupiedSeats();
        if (occupied > 0 && m_peopleManager && m_seatGrid)
        {
            glm::vec3 doorPos(-8.5f, 1.7f, -5.0f);  
            m_peopleManager->spawnPeopleRandom(*m_seatGrid, doorPos, 1, occupied);
            LOG_INFO("Door fully open - spawned " + std::to_string(m_peopleManager->getPeopleCount()) + 
                     " people for " + std::to_string(occupied) + " occupied seats");
        }
        peopleSpawned = true;
    }
    
    
    if (m_peopleManager && m_peopleManager->allSeated())
    {
        peopleSpawned = false;  
        enterState(AppState::Projection);
    }
}

void Application::updateProjectionState()
{
    
    const float DOOR_CLOSE_DELAY = 1.5f;
    
    if (m_stateTimer < DOOR_CLOSE_DELAY)
    {
        
        return;
    }
    
    
    static bool filmStarted = false;
    if (!filmStarted && m_stateTimer >= DOOR_CLOSE_DELAY)
    {
        if (m_screen) m_screen->startPlayback();
        LOG_INFO("Door fully closed - film playback started (20 seconds)");
        filmStarted = true;
    }
    
    
    if (m_screen && !m_screen->isPlaying() && filmStarted)
    {
        filmStarted = false;  
        enterState(AppState::Exiting);
    }
}

void Application::updateExitingState()
{
    
    if (m_peopleManager && m_peopleManager->allExited())
    {
        enterState(AppState::Reset);
    }
}

void Application::updateResetState()
{
    
    const float DOOR_CLOSE_DELAY = 1.5f;
    
    if (m_stateTimer < DOOR_CLOSE_DELAY)
    {
        
        static bool doorClosing = false;
        if (!doorClosing)
        {
            if (m_door) m_door->close();
            LOG_INFO("Door closing... preparing reset");
            doorClosing = true;
        }
        return;
    }
    
    
    static bool doorClosing = false;
    doorClosing = false;  
    
    
    if (m_peopleManager) m_peopleManager->clear();
    
    
    if (m_seatGrid)
    {
        for (int row = 0; row < SeatGrid::ROWS; ++row)
        {
            for (int col = 0; col < SeatGrid::COLS; ++col)
            {
                Seat* seat = m_seatGrid->getSeat(row, col);
                if (seat) seat->state = SeatState::Free;
            }
        }
    }
    
    
    if (m_screen) m_screen->stopAndResetToWhite();
    
    LOG_INFO("Reset complete - returning to Booking");
    enterState(AppState::Booking);
}

void Application::setLightingForState(AppState state)
{
    if (!m_scene) return;
    
    switch (state)
    {
        case AppState::Booking:
        case AppState::Entering:
        case AppState::Exiting:
        case AppState::Reset:
            m_scene->getRoomLight().enabled = true;
            m_scene->getScreenLight().enabled = false;
            break;
            
        case AppState::Projection:
            m_scene->getRoomLight().enabled = false;
            m_scene->getScreenLight().enabled = true;
            break;
    }
}

void Application::handleEnterKey()
{
    if (Input::isKeyPressed(GLFW_KEY_ENTER))
    {
        if (m_currentState == AppState::Booking)
        {
            int occupied = countOccupiedSeats();
            if (occupied > 0)
            {
                LOG_INFO("ENTER pressed - starting cinema cycle with " + 
                         std::to_string(occupied) + " occupied seats");
                enterState(AppState::Entering);
            }
            else
            {
                LOG_INFO("ENTER pressed - no occupied seats, please reserve or purchase seats first");
            }
        }
    }
}

int Application::countOccupiedSeats() const
{
    if (!m_seatGrid) return 0;
    
    int count = 0;
    for (int row = 0; row < SeatGrid::ROWS; ++row)
    {
        for (int col = 0; col < SeatGrid::COLS; ++col)
        {
            Seat* seat = m_seatGrid->getSeat(row, col);
            if (seat && (seat->state == SeatState::Reserved || seat->state == SeatState::Purchased))
            {
                count++;
            }
        }
    }
    return count;
}

const char* Application::stateToString(AppState state) const
{
    switch (state)
    {
        case AppState::Booking: return "Booking";
        case AppState::Entering: return "Entering";
        case AppState::Projection: return "Projection";
        case AppState::Exiting: return "Exiting";
        case AppState::Reset: return "Reset";
        default: return "Unknown";
    }
}

void Application::handlePurchaseKeys()
{
    for (int key = GLFW_KEY_1; key <= GLFW_KEY_9; ++key)
    {
        if (Input::isKeyPressed(key))
        {
            int groupSize = key - GLFW_KEY_0;
            bool success = m_seatGrid->purchaseAdjacent(groupSize);
            
            if (success)
            {
                LOG_INFO("Purchased " + std::to_string(groupSize) + " adjacent seats");
            }
            else
            {
                LOG_INFO("Cannot purchase " + std::to_string(groupSize) + " adjacent seats");
            }
            break;
        }
    }
}

void Application::handleSeatPicking()
{
    if (!Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        return;
    
    int screenWidth = m_window->width();
    int screenHeight = m_window->height();
    double mouseX = screenWidth / 2.0;
    double mouseY = screenHeight / 2.0;
    
    float aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
    glm::mat4 view = m_camera->viewMatrix();
    glm::mat4 projection = m_camera->projectionMatrix(aspect);
    glm::vec3 camPos = m_camera->getPosition();
    
    Ray ray = m_rayPicker->screenPointToRay(mouseX, mouseY, screenWidth, screenHeight, 
                                             view, projection, camPos);
    
    Seat* pickedSeat = m_rayPicker->pickSeat(ray, *m_seatGrid);
    
    if (pickedSeat)
    {
        if (pickedSeat->state == SeatState::Free)
        {
            pickedSeat->state = SeatState::Reserved;
            LOG_INFO("Seat [" + std::to_string(pickedSeat->row) + "," + 
                     std::to_string(pickedSeat->col) + "] -> Reserved");
        }
        else if (pickedSeat->state == SeatState::Reserved)
        {
            pickedSeat->state = SeatState::Free;
            LOG_INFO("Seat [" + std::to_string(pickedSeat->row) + "," + 
                     std::to_string(pickedSeat->col) + "] -> Free");
        }
    }
}

void Application::handleRenderToggles()
{
    
    if (Input::isKeyPressed(GLFW_KEY_V))
    {
        m_depthTestEnabled = !m_depthTestEnabled;
        if (m_depthTestEnabled)
        {
            glEnable(GL_DEPTH_TEST);
            LOG_INFO("[RENDER] DepthTest: ON");
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
            LOG_INFO("[RENDER] DepthTest: OFF");
        }
    }
    
    
    if (Input::isKeyPressed(GLFW_KEY_C))
    {
        m_cullingEnabled = !m_cullingEnabled;
        if (m_cullingEnabled)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);
            LOG_INFO("[RENDER] Culling: ON (GL_BACK, CCW)");
        }
        else
        {
            glDisable(GL_CULL_FACE);
            LOG_INFO("[RENDER] Culling: OFF");
        }
    }
}

void Application::shutdown()
{
    LOG_INFO("Shutting down Application...");
    m_running = false;
    
    if (m_hud)
    {
        m_hud->shutdown();
        m_hud.reset();
    }
    
    m_door.reset();
    m_screen.reset();
    m_peopleManager.reset();
    
    if (m_crosshair)
    {
        m_crosshair->cleanup();
        m_crosshair.reset();
    }
    
    m_rayPicker.reset();
    m_seatGrid.reset();
    m_scene.reset();
    
    if (m_debugCube)
    {
        m_debugCube->cleanup();
        m_debugCube.reset();
    }
    
    if (m_seatMesh)
    {
        m_seatMesh->cleanup();
        m_seatMesh.reset();
    }
    
    if (m_humanMesh)
    {
        m_humanMesh->cleanup();
        m_humanMesh.reset();
    }
    
    m_humanShader.reset();
    m_phongShader.reset();
    m_basicShader.reset();
    m_camera.reset();
    
    if (m_window)
    {
        m_window->shutdown();
        m_window.reset();
    }
    
    m_frameLimiter.reset();
    
    LOG_INFO("Application shut down successfully");
}
