#include "../Header/Input.h"
#include <GLFW/glfw3.h>
#include <cstring>

GLFWwindow* Input::s_window = nullptr;

double Input::s_mouseX = 0.0;
double Input::s_mouseY = 0.0;
double Input::s_lastMouseX = 0.0;
double Input::s_lastMouseY = 0.0;
float Input::s_mouseDeltaX = 0.0f;
float Input::s_mouseDeltaY = 0.0f;
bool Input::s_firstMouse = true;
bool Input::s_cursorVisible = false;  

bool Input::s_currentKeys[512] = { false };
bool Input::s_previousKeys[512] = { false };
bool Input::s_currentMouseButtons[8] = { false };
bool Input::s_previousMouseButtons[8] = { false };

void Input::init(GLFWwindow* window)
{
    s_window = window;
    s_firstMouse = true;

    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    
    glfwGetCursorPos(window, &s_mouseX, &s_mouseY);
    s_lastMouseX = s_mouseX;
    s_lastMouseY = s_mouseY;

    
    std::memset(s_currentKeys, 0, sizeof(s_currentKeys));
    std::memset(s_previousKeys, 0, sizeof(s_previousKeys));
    std::memset(s_currentMouseButtons, 0, sizeof(s_currentMouseButtons));
    std::memset(s_previousMouseButtons, 0, sizeof(s_previousMouseButtons));
}

void Input::update()
{
    if (!s_window) return;

    
    std::memcpy(s_previousKeys, s_currentKeys, sizeof(s_currentKeys));
    std::memcpy(s_previousMouseButtons, s_currentMouseButtons, sizeof(s_currentMouseButtons));

    
    for (int i = 0; i < 512; ++i)
    {
        s_currentKeys[i] = (glfwGetKey(s_window, i) == GLFW_PRESS);
    }

    
    for (int i = 0; i < 8; ++i)
    {
        s_currentMouseButtons[i] = (glfwGetMouseButton(s_window, i) == GLFW_PRESS);
    }

    
    glfwGetCursorPos(s_window, &s_mouseX, &s_mouseY);

    if (s_firstMouse)
    {
        s_lastMouseX = s_mouseX;
        s_lastMouseY = s_mouseY;
        s_firstMouse = false;
    }

    s_mouseDeltaX = static_cast<float>(s_mouseX - s_lastMouseX);
    s_mouseDeltaY = static_cast<float>(s_mouseY - s_lastMouseY);

    s_lastMouseX = s_mouseX;
    s_lastMouseY = s_mouseY;
}

bool Input::isKeyDown(int key)
{
    if (key < 0 || key >= 512) return false;
    return s_currentKeys[key];
}

bool Input::isKeyPressed(int key)
{
    if (key < 0 || key >= 512) return false;
    return s_currentKeys[key] && !s_previousKeys[key];
}

double Input::getMouseX()
{
    return s_mouseX;
}

double Input::getMouseY()
{
    return s_mouseY;
}

float Input::getMouseDeltaX()
{
    return s_mouseDeltaX;
}

float Input::getMouseDeltaY()
{
    return s_mouseDeltaY;
}

bool Input::isMouseButtonDown(int button)
{
    if (button < 0 || button >= 8) return false;
    return s_currentMouseButtons[button];
}

bool Input::isMouseButtonPressed(int button)
{
    if (button < 0 || button >= 8) return false;
    return s_currentMouseButtons[button] && !s_previousMouseButtons[button];
}

void Input::toggleCursorMode()
{
    if (!s_window) return;
    
    s_cursorVisible = !s_cursorVisible;
    
    if (s_cursorVisible)
    {
        glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else
    {
        glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    
    
    s_firstMouse = true;
}

bool Input::isCursorVisible()
{
    return s_cursorVisible;
}
