#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "rendering/Shader.hpp"
#include "rendering/Buffers.hpp"
#include "rendering/VertexArray.hpp"

#include "util/Texture2D.hpp"
#include "util/ErrorHandling.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int WINDOW_WIDTH = 1600;
int WINDOW_HEIGHT = 900;

GLFWwindow* InitAll();

int main() {
    GLFWwindow* window = InitAll();

    if(!window) return -1;

    Shader* t_GlobalShader = new Shader(L"src/shaders/global/vertex.shader", L"src/shaders/global/fragment.shader");
    VertexArray* VAO;
    IndexBuffer* IBO;

    Texture2D t_Tex("assets/textures/catpfp.png");

    //Temporary Lambda to render a mesh
    const auto t_Render = [](VertexArray* VAO, IndexBuffer* IBO, Shader* Shdr) {
        VAO->Bind();
        IBO->Bind();
        Shdr->Bind();
        GLCall(glDrawElements(GL_TRIANGLES, IBO->GetCount(), GL_UNSIGNED_INT, nullptr));
    };

    //Temporary scope to show how to initialize vao and ibo in a mesh object
    {
        //All data passed in as parameters
        float t_Vertices[] = {
            //POSITION           //TEXCOORD   //COLOR
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
             0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f,

             0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f,
             0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
             0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f
        };

        unsigned int t_Layout[] = { 3, 2, 3 };

        unsigned int t_Indices[] = {
            0, 1, 2, 2, 3, 0,
            4, 7, 6, 6, 5, 4,
            8, 9, 10, 10, 11, 8,
            12, 15, 14, 14, 13, 12,
            16, 17, 18, 18, 19, 16,
            20, 23, 22, 22, 21, 20
        };
    
        VertexBuffer VBO(t_Vertices);
        VAO = new VertexArray(VBO, t_Layout);
        IBO = new IndexBuffer(t_Indices);
    }

    //DIFF FOR EACH MODEL
    glm::mat4 t_Model(1.0f);

    //DIFF FOR EACH CAMERA
    glm::mat4 t_View(1.0f);
    t_View = glm::translate(t_View, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 t_Projection = glm::perspective(glm::radians(70.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    t_GlobalShader->SetUniformMat4f("u_Projection", t_Projection);

    float k = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         
        //Binding texture to a slot and setting the uniform to that slot
        t_Tex.Bind(0);  
        t_GlobalShader->SetUniform1i("v_Texture", 0); 
        
        glm::vec3 moved = glm::vec3(2 * glm::sin(glm::radians(k)), 0.0f, 0.0f);
         
        t_View = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f) + moved, glm::vec3(0.0f, 0.0f, -1.0f) + moved, glm::vec3(0.0f, 1.0f, 0.0f));

        t_GlobalShader->SetUniformMat4f("u_Model", t_Model);
        t_GlobalShader->SetUniformMat4f("u_View", t_View);

        t_Render(VAO, IBO, t_GlobalShader);

        glfwSwapBuffers(window);
        glfwPollEvents();

        k += 1.0f;
        if (k >= 360) k -= 360;
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

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return window;
}

//CALLBACKS