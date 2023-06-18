#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct WindowData {
    int width;
    int height;
    bool resized;
};

class OpenGLApp {
public:
    OpenGLApp(GLFWwindow* window, int windowWidth, int windowHeight);

    void onResize(int width, int height);

    virtual void run() = 0;
protected:
    GLFWwindow* m_Window;
    WindowData m_WindowData;
};