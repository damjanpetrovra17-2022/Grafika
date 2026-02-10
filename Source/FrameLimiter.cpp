#include "../Header/FrameLimiter.h"
#include "../Header/Log.h"
#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>
#include <iomanip>
#include <sstream>

FrameLimiter::FrameLimiter(double targetFPS)
    : m_targetFrameTime(1.0 / targetFPS)
    , m_frameStartTime(0.0)
    , m_deltaTime(0.0f)
    , m_fpsTimer(0.0)
    , m_frameCount(0)
    , m_currentFPS(0.0)
{
}

void FrameLimiter::beginFrame()
{
    m_frameStartTime = glfwGetTime();
}

void FrameLimiter::endFrame()
{
    double frameEndTime = glfwGetTime();
    double frameTime = frameEndTime - m_frameStartTime;

    
    if (frameTime < m_targetFrameTime)
    {
        double sleepTime = m_targetFrameTime - frameTime;
        
        
        if (sleepTime > 0.001)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(
                static_cast<long long>((sleepTime - 0.001) * 1000000.0)
            ));
        }

        
        while (glfwGetTime() - m_frameStartTime < m_targetFrameTime)
        {
            
        }
    }

    
    double actualFrameEnd = glfwGetTime();
    m_deltaTime = static_cast<float>(actualFrameEnd - m_frameStartTime);

    
    m_frameCount++;
    m_fpsTimer += m_deltaTime;
    
    if (m_fpsTimer >= 1.0)
    {
        m_currentFPS = static_cast<double>(m_frameCount) / m_fpsTimer;
        
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << "FPS: " << m_currentFPS;
        LOG_INFO(oss.str());
        
        m_frameCount = 0;
        m_fpsTimer = 0.0;
    }
}

float FrameLimiter::getDeltaTime() const
{
    return m_deltaTime;
}

double FrameLimiter::getCurrentFPS() const
{
    return m_currentFPS;
}
