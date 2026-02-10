#pragma once

#include "AppState.h"
#include <memory>

class Window;
class FrameLimiter;
class Camera;
class DebugCube;
class SeatMesh;
class HumanMesh;
class Shader;
class Scene;
class SeatGrid;
class RayPicker;
class Crosshair;
class PeopleManager;
class Screen;
class Door;
class HUD;

class Application
{
public:
    Application();
    ~Application();

    bool init();
    void run();
    void shutdown();

private:

void handleSeatPicking();
void handlePurchaseKeys();
void handleEnterKey();
void handleRenderToggles();
    
    
    void updateStateMachine(float deltaTime);
    void enterState(AppState newState);
    int countOccupiedSeats() const;
    const char* stateToString(AppState state) const;
    
    
    void updateBookingState();
    void updateEnteringState();
    void updateProjectionState();
    void updateExitingState();
    void updateResetState();
    
    
    void setLightingForState(AppState state);
    
    bool m_running;
    AppState m_currentState;
    float m_stateTimer;
    float m_debugPrintTimer;
    
    
    bool m_depthTestEnabled;
    bool m_cullingEnabled;
    
    std::unique_ptr<Window> m_window;
    std::unique_ptr<FrameLimiter> m_frameLimiter;
    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<DebugCube> m_debugCube;
    std::unique_ptr<SeatMesh> m_seatMesh;
    std::unique_ptr<HumanMesh> m_humanMesh;
    std::unique_ptr<Shader> m_basicShader;
    std::unique_ptr<Shader> m_phongShader;
    std::unique_ptr<Shader> m_humanShader;
    std::unique_ptr<Scene> m_scene;
    std::unique_ptr<SeatGrid> m_seatGrid;
    std::unique_ptr<RayPicker> m_rayPicker;
    std::unique_ptr<Crosshair> m_crosshair;
    std::unique_ptr<PeopleManager> m_peopleManager;
    std::unique_ptr<Screen> m_screen;
    std::unique_ptr<Door> m_door;
    std::unique_ptr<HUD> m_hud;
};
