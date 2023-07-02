#include "OpenGLApp.hpp"
#include <iostream>

OpenGLApp::OpenGLApp() : m_Window(nullptr), m_WindowData({ 0, 0, false }) { }

void OpenGLApp::initialize(int width, int height) {
    m_WindowData.width = width;
    m_WindowData.height = height;

    if (!glfwInit()) {
        std::cout << "GLFW initialization failed!\n";
        __debugbreak();
    }

    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);

    if (!m_Window) {
        glfwTerminate();
        std::cout << "GLFW window initialization failed!\n";
        __debugbreak();
    }


    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(m_Window, this);
    glfwSetFramebufferSizeCallback(m_Window, &OpenGLApp::glfwResizeCallback);

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        std::cout << "GLEW initialization failed!\n";
        __debugbreak();
    }

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
}

void OpenGLApp::glfwResizeCallback(GLFWwindow* window, int width, int height) {
    OpenGLApp* app = static_cast<OpenGLApp*>(glfwGetWindowUserPointer(window));
    app->onResize(window, width, height);
}

void OpenGLApp::onResize(GLFWwindow* window, int width, int height) {
    m_WindowData.width = width;
    m_WindowData.height = height;
    m_WindowData.resized = true;
    glViewport(0, 0, width, height);
}