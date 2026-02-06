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

    // Sleep to enforce frame cap (but leave some margin for spin-wait)
    if (frameTime < m_targetFrameTime)
    {
        double sleepTime = m_targetFrameTime - frameTime;
        
        // Sleep for most of the remaining time (leave 1ms for spin-wait to avoid oversleep)
        if (sleepTime > 0.001)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(
                static_cast<long long>((sleepTime - 0.001) * 1000000.0)
            ));
        }

        // Spin-wait for the remaining time to be precise
        while (glfwGetTime() - m_frameStartTime < m_targetFrameTime)
        {
            // Busy wait for precise timing
        }
    }

    // Calculate actual delta time after frame limiting
    double actualFrameEnd = glfwGetTime();
    m_deltaTime = static_cast<float>(actualFrameEnd - m_frameStartTime);

    // FPS tracking - print once per second
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
