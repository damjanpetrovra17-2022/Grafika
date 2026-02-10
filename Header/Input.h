#pragma once

struct GLFWwindow;

class Input
{
public:
    static void init(GLFWwindow* window);
    static void update();

    
    static bool isKeyDown(int key);
    static bool isKeyPressed(int key);  

    
    static double getMouseX();
    static double getMouseY();
    static float getMouseDeltaX();
    static float getMouseDeltaY();

    
    static bool isMouseButtonDown(int button);
    static bool isMouseButtonPressed(int button);  
    
    
    static void toggleCursorMode();
    static bool isCursorVisible();

private:
    static GLFWwindow* s_window;
    
    static double s_mouseX;
    static double s_mouseY;
    static double s_lastMouseX;
    static double s_lastMouseY;
    static float s_mouseDeltaX;
    static float s_mouseDeltaY;
    static bool s_firstMouse;
    static bool s_cursorVisible;

    static bool s_currentKeys[512];
    static bool s_previousKeys[512];
    static bool s_currentMouseButtons[8];
    static bool s_previousMouseButtons[8];
};
