#pragma once

class Time
{
public:
    static void init();
    static void update();
    
    static float deltaTime();
    static double now();

private:
    static double s_lastFrameTime;
    static float s_deltaTime;
};
