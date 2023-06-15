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

int WINDOW_WIDTH = 1600;
int WINDOW_HEIGHT = 900;
bool g_Resized = false;

GLFWwindow* InitAll();
void c_OnResize(GLFWwindow* window, int width, int height);

int main() {
    GLFWwindow* window = InitAll();

    if(!window) return -1;

    Shader t_GlobalShader(L"src/shaders/global/vertex.shader", L"src/shaders/global/fragment.shader"); 
    Shader t_LightSrcShader(L"src/shaders/light_source/vertex.shader", L"src/shaders/light_source/fragment.shader");

    //BASIC MESH WITH DIFF AND SPEC MAP
    VertexArray* VAO; 
    IndexBuffer* IBO; 
    Texture2D t_Tex("assets/textures/catpfp.png");
    Texture2D t_Spec("assets/textures/testspec.png");
 
    //MESH FOR LIGHTCUBE
    VertexArray* VAOL;

    Timer t_Timer;
    Camera t_Camera;
    CameraHandler t_CamHandler(t_Camera);

    PointLight* t_Light;
    glm::mat4 t_ModelL;

    //Temporary Lambda to render a mesh
    const auto t_Render = [](const VertexArray& VAO, const IndexBuffer& IBO, const Shader& Shdr) {
        VAO.Bind();
        IBO.Bind(); 
        Shdr.Bind();
        GLCall(glDrawElements(GL_TRIANGLES, IBO.GetCount(), GL_UNSIGNED_INT, nullptr));
    };

    //Temporary scope to show how to initialize vao and ibo in a mesh object
    {
        //All data passed in as parameters
        float t_Vertices[] = {
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

        float t_VerticesL[] = {
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
     
        unsigned int t_LayoutL[] = { 3 };

        VertexBuffer VBOL(t_VerticesL);
        VAOL = new VertexArray(VBOL, t_LayoutL);

        //LIGHTING
        glm::vec3 t_LightPos(3.0f, 0.0f, 4.0f); 
        glm::vec3 t_Color(1.0f, 1.0f, 1.0f);

        t_Light = new PointLight(t_LightPos, t_Color, 1.0f);
        t_Light->SetUniforms(t_GlobalShader);

        //LIGHT CUBE MODEL
        t_ModelL = glm::scale(glm::translate(glm::mat4(1.0f), t_LightPos), glm::vec3(0.2f));
        t_LightSrcShader.SetUniformMat4f("u_Model", t_ModelL);
        t_LightSrcShader.SetUniform3fv("u_Color", t_Color);
    }

    //DIFF FOR EACH BOX MODEL
    glm::mat4 t_Model(1.0f);

    //Temp positions to render multiple boxes
    const glm::vec3 t_BoxPositions[] = {
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

    float t_DeltaTime;
    float k = 0.5f;
 
    while (!glfwWindowShouldClose(window)) { 
        if (g_Resized) t_Camera.ResetProjectionMat(WINDOW_WIDTH, WINDOW_HEIGHT);
        t_DeltaTime = t_Timer.GetDeltaTime();
        
        glClearColor(0.0f, 0.05f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
        //CAMERA UPDATES 
        t_CamHandler.HandleEvents(window, t_DeltaTime);
        t_Camera.UpdateUniforms("u_View", "u_Projection", "u_ViewPos", t_GlobalShader);
        t_Camera.UpdateUniforms("u_View", "u_Projection", t_LightSrcShader);

        //Binding texture to a slot and setting the uniform to that slot
        t_Tex.Bind(0); 
        t_GlobalShader.SetUniform1i("u_Texture", 0); 
        t_Spec.Bind(1);
        t_GlobalShader.SetUniform1i("u_SpecMap", 1);

        //Rendering box at all positions
        for (const auto& pos : t_BoxPositions) {
            t_Model = glm::translate(glm::rotate(glm::mat4(1.0f), glm::radians(k), pos), pos);
            t_GlobalShader.SetUniformMat4f("u_Model", t_Model); 
            t_Render(*VAO, *IBO, t_GlobalShader);
        }

        //Render the light source box
        t_Render(*VAOL, *IBO, t_LightSrcShader);
        
        glfwSwapBuffers(window); 
        glfwPollEvents();

        k += 0.5f;
        if (k >= 360.0f) k -= 360.0f;
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, c_OnResize);

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

void c_OnResize(GLFWwindow* window, int width, int height) {
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    g_Resized = true;
    glViewport(0, 0, width, height);
}