#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "util/ErrorHandling.hpp"
#include "util/Timer.hpp"

#include "rendering/Shader.hpp"
#include "rendering/Buffers.hpp"
#include "rendering/VertexArray.hpp"

#include "textures/Texture2D.hpp"

#include "scene/Camera.hpp"
#include "scene/CameraHandler.hpp"

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

    //CAMERA AND TIMER
    Timer t_Timer; 
    Camera t_Camera; 
    CameraHandler t_CamHandler(t_Camera);

    //DIFF FOR EACH MODEL
    glm::mat4 t_Model(1.0f);

    float t_DeltaTime;

    while (!glfwWindowShouldClose(window)) {
        t_DeltaTime = t_Timer.GetDeltaTime();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Binding texture to a slot and setting the uniform to that slot
        t_Tex.Bind(0);  
        t_GlobalShader->SetUniform1i("v_Texture", 0); 

        //TEMP MODEL MATRIX
        t_GlobalShader->SetUniformMat4f("u_Model", t_Model);

        //CAMERA UPDATES
        t_Camera.UpdateUniforms("u_View", "u_Projection", *t_GlobalShader);
        t_CamHandler.HandleEvents(window, t_DeltaTime);

        //RENDERING
        t_Render(VAO, IBO, t_GlobalShader);

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

    //CHANGE THE CONTEXT_VERSION IF DEVICE DOESNT SUPPORT 4.5
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

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

    //ENABLING OPENGL FLAGS
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE); 

    return window;
}

//CALLBACKS