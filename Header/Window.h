#pragma once

struct GLFWwindow;
struct GLFWmonitor;

class Window
{
public:
    Window();
    ~Window();

    bool init();
    void shutdown();

    GLFWwindow* handle() const;
    int width() const;
    int height() const;
    bool shouldClose() const;

    void pollEvents();
    void swapBuffers();

private:
    GLFWwindow* m_window;
    int m_width;
    int m_height;

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
