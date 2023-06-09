#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "rendering/Shader.hpp"
#include "rendering/Buffers.hpp"
#include "rendering/VertexArray.hpp"

GLFWwindow* InitAll();

int main() {
    GLFWwindow* window = InitAll();
    Shader* t_GlobalShader = new Shader("src/shaders/global/vertex.shader", "src/shaders/global/fragment.shader");
    VertexArray* VAO; 
    IndexBuffer* IBO;  

    //Temporary Lambda to render a mesh
    const auto t_TempRender = [](VertexArray* VAO, IndexBuffer* IBO, Shader* Shdr) {
        VAO->Bind(); 
        IBO->Bind(); 
        Shdr->Bind(); 
        glDrawElements(GL_TRIANGLES, IBO->GetCount(), GL_UNSIGNED_INT, nullptr); 
    }; 

    //Temporary scope to show how to initialize a mesh
    {
        //All data passed in as parameters

        float t_Vertices[] = {
            //POSITION          //COLOR
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
             0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
             1.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f
        };

        unsigned int t_Layout[] = { 3, 3 };

        unsigned int t_Indices[] = {
            0, 1, 2,
            1, 2, 3,
        };

        VertexBuffer VBO(t_Vertices);
        VAO = new VertexArray(VBO, t_Layout);
        IBO = new IndexBuffer(t_Indices);
    }

    while (!glfwWindowShouldClose(window)) { 
        glClear(GL_COLOR_BUFFER_BIT);

        t_TempRender(VAO, IBO, t_GlobalShader);

        glfwSwapBuffers(window); 
        glfwPollEvents();  
    }

    glfwTerminate();
    return 0;
}

//SMALL ABSTRACTIONS

GLFWwindow* InitAll() {
    if (!glfwInit()) {
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(1600, 900, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return nullptr;
    }

    glfwSwapInterval(0);
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return nullptr;
    }

    return window;
}