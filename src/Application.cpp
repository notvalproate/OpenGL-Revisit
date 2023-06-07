#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "rendering/Shader.hpp"
//#include "rendering/VertexBuffer.hpp"
//#include "rendering/IndexBuffer.hpp"
#include "rendering/Buffers.hpp"

int main() {
    if (!glfwInit()) {
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(1600, 900, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwSwapInterval(0);
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return -1;
    }

    std::vector<float> t_Vertices = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f
    };

    std::vector<unsigned int> t_Indices = { 
        0, 1, 2, 3
    };

    //VERTEX BUFFER
    VertexBuffer VBO(t_Vertices);
    VBO.Bind();

    //VERTEX ARRAY
    unsigned int va;
    glGenVertexArrays(1, &va);
    glBindVertexArray(va);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //INDEX BUFFER
    IndexBuffer IBO(t_Indices);
    IBO.Bind();

    //SHADER
    Shader t_GlobalShader("src/shaders/global/vertex.shader", "src/shaders/global/fragment.shader");
    t_GlobalShader.Bind();

    while (!glfwWindowShouldClose(window)) { 
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawElements(GL_TRIANGLE_STRIP, t_Indices.size(), GL_UNSIGNED_INT, nullptr);
         
        glfwSwapBuffers(window); 
        glfwPollEvents();  
    }

    glfwTerminate();
    return 0;
}