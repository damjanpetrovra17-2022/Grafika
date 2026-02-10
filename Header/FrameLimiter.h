#pragma once

class FrameLimiter
{
public:
    FrameLimiter(double targetFPS = 75.0);

    void beginFrame();
    void endFrame();

    float getDeltaTime() const;
    double getCurrentFPS() const;

private:
    double m_targetFrameTime;
    double m_frameStartTime;
    float m_deltaTime;
    
    
    double m_fpsTimer;
    int m_frameCount;
    double m_currentFPS;
};
