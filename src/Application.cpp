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

#include "lighting/PointLight.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*
CHANGED NAMING SCHEMES THAT I USE: 

Functions:
    - in lower Camel Case
Namespaces:
    - in lower case
Variables:
    - in lower Camel Case
Variable Prefixes:
    - Local Variables  : none
    - Private Members  : m_
    - Global Variables : g_
    - Static Variables : s_
    - Arguments        : a_
*/


int g_WindowWidth = 1600;
int g_WindowHeight = 900;
bool g_Resized = false;

GLFWwindow* initializeAll();
void onResize(GLFWwindow* window, int width, int height);

int main() {
    GLFWwindow* window = initializeAll();

    if(!window) return -1;

    Shader globalShader(L"src/shaders/global/vertex.shader", L"src/shaders/global/fragment.shader"); 
    Shader lightSourceShader(L"src/shaders/light_source/vertex.shader", L"src/shaders/light_source/fragment.shader");

    Timer timer; 
    Camera camera; 
    CameraHandler camHandler(camera); 

    //BASIC MESH WITH DIFF AND SPEC MAP
    VertexArray* vao; 
    IndexBuffer* ibo; 
    Texture2D diffuseMap("assets/textures/catpfp.png");
    Texture2D specularMap("assets/textures/catpfp.png");
 
    //MESH FOR LIGHTCUBE
    VertexArray* vaol;

    //LIGHTING
    PointLight* pointLight;
    glm::mat4 lightModel;

    //Temporary Lambda to render a mesh
    const auto renderMesh = [](const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) {
        vao.bind();
        ibo.bind(); 
        shader.bind();
        GLCall(glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr));
    };

    //Temporary scope to show how to initialize vao and ibo in a mesh object
    {
        //All data passed in as parameters
        float vertices[] = {
            //POSITION           //TEXCOORD  //NORMALS
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  0.0f,  1.0f,
                                             		 	    
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
                                             		 	    
             0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
                                             		 	    
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
                                             			    
            -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
                                             			    
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f,  0.0f
        };
        
        unsigned int layout[] = { 3, 2, 3 };

        unsigned int indices[] = {
            0, 1, 2, 2, 3, 0,
            4, 7, 6, 6, 5, 4,
            8, 9, 10, 10, 11, 8,
            12, 15, 14, 14, 13, 12,
            16, 17, 18, 18, 19, 16,
            20, 23, 22, 22, 21, 20
        };
    
        VertexBuffer VBO(vertices);
        vao = new VertexArray(VBO, layout);
        ibo = new IndexBuffer(indices);

        float lightVertices[] = {
            //POSITION          
            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,

             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f
        };
     
        unsigned int lightLayout[] = { 3 };

        VertexBuffer vbol(lightVertices);
        vaol = new VertexArray(vbol, lightLayout);

        //LIGHTING
        glm::vec3 lightPos(3.0f, 0.0f, 4.0f); 
        glm::vec3 color(1.0f, 0.7f, 0.8f);

        pointLight = new PointLight(lightPos, color, 1.0f);
        pointLight->UpdateUniforms(globalShader);

        //LIGHT CUBE MODEL
        lightModel = glm::scale(glm::translate(glm::mat4(1.0f), lightPos), glm::vec3(0.2f));
        lightSourceShader.setUniformMat4f("u_Model", lightModel);
        lightSourceShader.setUniform3fv("u_Color", color);
    }

    //DIFF FOR EACH BOX MODEL
    glm::mat4 model(1.0f);

    //Temp positions to render multiple boxes
    const glm::vec3 boxPositions[] = {
        glm::vec3(0.1f, 0.0f, 0.0f),
        glm::vec3(5.0f, 4.0f, -1.0f),
        glm::vec3(-2.0f, 7.0f, 3.0f),
        glm::vec3(6.0f, -3.0f, 6.0f),
        glm::vec3(2.0f, -6.0f, -3.0f),
        glm::vec3(8.0f, 3.0f, -10.0f),
        glm::vec3(-8.0f, 2.0f, 6.0f),
        glm::vec3(3.0f, -5.0f, 8.0f),
        glm::vec3(9.0f, 8.0f, -2.0f)
    };


    float k = 0.5f;
    while (!glfwWindowShouldClose(window)) { 
        if (g_Resized) camera.resetViewport(g_WindowWidth, g_WindowHeight);
        float deltaTime = timer.getDeltaTime();
        
        glClearColor(0.0f, 0.05f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
        //CAMERA UPDATES 
        camHandler.handleEvents(window, deltaTime);
        camera.updateUniforms("u_View", "u_Projection", "u_ViewPos", globalShader);
        camera.updateUniforms("u_View", "u_Projection", lightSourceShader);

        //Binding texture to a slot and setting the uniform to that slot
        diffuseMap.bind(0); 
        globalShader.setUniform1i("u_Texture", 0); 
        specularMap.bind(1);
        globalShader.setUniform1i("u_SpecMap", 1);

        //Rendering box at all positions
        float i = 10.0f;
        for (const auto& pos : boxPositions) {
            model = glm::translate(glm::rotate(glm::mat4(1.0f), glm::radians(i), pos), pos);
            globalShader.setUniformMat4f("u_Model", model); 
            renderMesh(*vao, *ibo, globalShader);
            i += 30.0f;
        }

        //Render the light source box
        glm::vec3 test = glm::vec3(10 * glm::sin(glm::radians(k)), 0.0f, 4.0f);
        pointLight->SetPosition(test, globalShader);
        lightModel = glm::scale(glm::translate(glm::mat4(1.0f), test), glm::vec3(0.2f));
        lightSourceShader.setUniformMat4f("u_Model", lightModel);
        renderMesh(*vaol, *ibo, lightSourceShader);
        
        glfwSwapBuffers(window); 
        glfwPollEvents();

        k += 0.5f;
        if (k >= 360.0f) k -= 360.0f;
    }

    delete vao, ibo, vaol, pointLight;

    glfwTerminate();
    return 0;
}

//SMALL ABSTRACTIONS

GLFWwindow* initializeAll() {

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, onResize);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return nullptr;
    }

    //ENABLING OPENGL FLAGS
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return window;
}   

//CALLBACKS

void onResize(GLFWwindow* window, int width, int height) {
    g_WindowWidth = width;
    g_WindowHeight = height;
    g_Resized = true;
    glViewport(0, 0, width, height);
}