#include "../Header/Window.h"
#include "../Header/Log.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

Window::Window()
    : m_window(nullptr)
    , m_width(0)
    , m_height(0)
{
}

Window::~Window()
{
    shutdown();
}

bool Window::init()
{
    LOG_INFO("Creating fullscreen window...");

    // Get the primary monitor
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    if (!primaryMonitor)
    {
        LOG_ERROR("Failed to get primary monitor!");
        return false;
    }

    // Get the current video mode of the primary monitor
    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
    if (!videoMode)
    {
        LOG_ERROR("Failed to get video mode!");
        return false;
    }

    m_width = videoMode->width;
    m_height = videoMode->height;

    // Request OpenGL 3.3 core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create fullscreen window using the monitor's current video mode
    m_window = glfwCreateWindow(m_width, m_height, "3D Cinema", primaryMonitor, nullptr);
    if (!m_window)
    {
        LOG_ERROR("Failed to create GLFW window!");
        return false;
    }

    // Make the OpenGL context current
    glfwMakeContextCurrent(m_window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK)
    {
        LOG_ERROR("Failed to initialize GLEW!");
        glfwDestroyWindow(m_window);
        m_window = nullptr;
        return false;
    }

    // Set up key callback for ESC handling
    glfwSetKeyCallback(m_window, keyCallback);

    // Disable VSync (we'll use our own frame limiter)
    glfwSwapInterval(0);

    LOG_INFO("Window created successfully: " + std::to_string(m_width) + "x" + std::to_string(m_height));
    LOG_INFO("OpenGL Version: " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));

    return true;
}

void Window::shutdown()
{
    if (m_window)
    {
        LOG_INFO("Destroying window...");
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

GLFWwindow* Window::handle() const
{
    return m_window;
}

int Window::width() const
{
    return m_width;
}

int Window::height() const
{
    return m_height;
}

bool Window::shouldClose() const
{
    return m_window ? glfwWindowShouldClose(m_window) : true;
}

void Window::pollEvents()
{
    glfwPollEvents();
}

void Window::swapBuffers()
{
    if (m_window)
    {
        glfwSwapBuffers(m_window);
    }
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void)scancode;
    (void)mods;

    // ESC key exits immediately
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}
