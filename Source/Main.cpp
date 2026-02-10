

#include "../Header/Application.h"
#include "../Header/Log.h"
#include <GLFW/glfw3.h>

int main()
{
    
    if (!glfwInit())
    {
        LOG_ERROR("GLFW initialization failed!");
        return -1;
    }

    
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
