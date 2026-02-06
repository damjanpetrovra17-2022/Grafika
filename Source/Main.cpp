// Phase 0: Project Setup - Minimal Application Skeleton
// This replaces the old 2D cinema code with the new engine structure

#include "../Header/Application.h"
#include "../Header/Log.h"
#include <GLFW/glfw3.h>

int main()
{
    // Initialize GLFW early (needed for Time class)
    if (!glfwInit())
    {
        LOG_ERROR("GLFW initialization failed!");
        return -1;
    }

    // Create and run application
    Application app;
    
    if (!app.init())
    {
        LOG_ERROR("Application initialization failed!");
        glfwTerminate();
        return -1;
    }

    app.run();
    app.shutdown();

    glfwTerminate();
    return 0;
}
