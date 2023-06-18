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
    OpenGLApp();

    void initialize(int windowWidth, int windowHeight);
    virtual void run() = 0;

protected:
    GLFWwindow* m_Window;
    WindowData m_WindowData;


    static void glfwResizeCallback(GLFWwindow* window, int width, int height);
    void onResize(GLFWwindow* window, int width, int height);
};