#include "../Header/AppTime.h"
#include <GLFW/glfw3.h>

// Initialize static members
double Time::s_lastFrameTime = 0.0;
float Time::s_deltaTime = 0.0f;

void Time::init()
{
    s_lastFrameTime = glfwGetTime();
    s_deltaTime = 0.0f;
}

void Time::update()
{
    double currentTime = glfwGetTime();
    s_deltaTime = static_cast<float>(currentTime - s_lastFrameTime);
    s_lastFrameTime = currentTime;
}

float Time::deltaTime()
{
    return s_deltaTime;
}

double Time::now()
{
    return glfwGetTime();
}
