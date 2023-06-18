#include "OpenGLApp.hpp"
#include <iostream>

OpenGLApp::OpenGLApp(GLFWwindow* window, int windowWidth, int windowHeight) : m_WindowData({ windowWidth, windowHeight, false }) {
    m_Window = window;
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        std::cout << "GLEW initialization failed!\n";
        __debugbreak();
    }

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLApp::onResize(int width, int height) {
    m_WindowData.width = width;
    m_WindowData.height = height;
    m_WindowData.resized = true;
    glViewport(0, 0, width, height);
}