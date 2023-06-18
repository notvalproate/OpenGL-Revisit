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

#include "OpenGLApp.hpp"

/*
CHANGED NAMING SCHEMES THAT I USE: 

Functions:
    - in lower Camel Case
Namespaces:
    - in lower case
Variables:
    - in lower Camel Case, Pascal Case if prefixed
Variable Prefixes:
    - Local Variables and Arguments  : none
    - Private Members  : m_
    - Static Variables : s_
*/

class Game : public OpenGLApp {
public:
    Game(GLFWwindow* window, int width, int height) : OpenGLApp(window, width, height) {};

    void run() override {
        Shader globalShader(L"src/shaders/global/vertex.shader", L"src/shaders/global/fragment.shader");
        Shader lightSourceShader(L"src/shaders/light_source/vertex.shader", L"src/shaders/light_source/fragment.shader");

        Timer timer;
        Camera camera;
        CameraHandler camHandler(camera);

        //BASIC MESH WITH DIFF AND SPEC MAP
        VertexArray* vao;
        IndexBuffer* ibo;

        //MESH FOR LIGHTCUBE
        VertexArray* vaol;

        //LIGHTING
        PointLightList& pointLights = PointLightList::getList();
        pointLights.setShader(&globalShader);

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
                //POSITION         //TEXCOORD  //NORMALS         //TEX INDEX
                -0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                 0.5f,-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                -0.5f,-0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 0.0f,-1.0f, 0.0f,
                 0.5f,-0.5f,-0.5f, 1.0f, 0.0f, 0.0f, 0.0f,-1.0f, 0.0f,
                 0.5f, 0.5f,-0.5f, 1.0f, 1.0f, 0.0f, 0.0f,-1.0f, 0.0f,
                -0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f,-1.0f, 0.0f,

                 0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                 0.5f,-0.5f,-0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                 0.5f, 0.5f,-0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,

                -0.5f,-0.5f, 0.5f, 0.0f, 0.0f,-1.0f, 0.0f, 0.0f, 0.0f,
                -0.5f,-0.5f,-0.5f, 1.0f, 0.0f,-1.0f, 0.0f, 0.0f, 0.0f,
                -0.5f, 0.5f,-0.5f, 1.0f, 1.0f,-1.0f, 0.0f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,-1.0f, 0.0f, 0.0f, 0.0f,

                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 0.5f, 0.5f,-0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                -0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

                -0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 0.0f,-1.0f, 0.0f, 0.0f,
                 0.5f,-0.5f, 0.5f, 1.0f, 0.0f, 0.0f,-1.0f, 0.0f, 0.0f,
                 0.5f,-0.5f,-0.5f, 1.0f, 1.0f, 0.0f,-1.0f, 0.0f, 0.0f,
                -0.5f,-0.5f,-0.5f, 0.0f, 1.0f, 0.0f,-1.0f, 0.0f, 0.0f
            };

            unsigned int layout[] = { 3, 2, 3, 1 };

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
            glm::vec3 color(1.0f, 1.0f, 1.0f);

            pointLights.addLight(0, lightPos, color, 1.0f, &lightSourceShader);

            color = glm::vec3(1.0f, 0.1f, 0.1f);
            pointLights.addLight(1, lightPos, color, 1.0f, &lightSourceShader);

            color = glm::vec3(0.0f, 0.5f, 1.0f);
            pointLights.addLight(2, lightPos, color, 1.0f, &lightSourceShader);
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

        Texture2D diffuseMap("assets/textures/crate.png", GL_NEAREST, GL_CLAMP_TO_EDGE);
        Texture2D specularMap("assets/textures/crate_spec.png", GL_NEAREST, GL_CLAMP_TO_EDGE);

        float k = 0.5f;
        while (!glfwWindowShouldClose(m_Window)) {
            if (m_WindowData.resized) camera.resetViewport(m_WindowData.width, m_WindowData.height);
            float deltaTime = timer.getDeltaTime();

            glClearColor(0.0f, 0.05f, 0.15f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //CAMERA UPDATES
            camHandler.handleEvents(m_Window, deltaTime);
            camera.updateUniforms("u_View", "u_Projection", "u_ViewPos", globalShader);
            camera.updateUniforms("u_View", "u_Projection", lightSourceShader);

            //Binding texture to a slot and setting the uniform to that slot
            diffuseMap.bind(0);
            globalShader.setUniform1i("u_Materials[0].diffusion", 0);
            specularMap.bind(1);
            globalShader.setUniform1i("u_Materials[0].specular", 1);
            globalShader.setUniform1f("u_Materials[0].shininess", 76.8f);

            //Rendering box at all positions
            float i = 10.0f;
            for (const auto& pos : boxPositions) {
                model = glm::translate(glm::rotate(glm::mat4(1.0f), glm::radians(i), pos), pos);
                globalShader.setUniformMat4f("u_Model", model);
                renderMesh(*vao, *ibo, globalShader);
                i += 30.0f;
            }

            //Render the lights
            float test = 8.0f * glm::sin(glm::radians(k));
            pointLights.setLightPosition(0, glm::vec3(test, 0.0f, 4.0f));
            renderMesh(*vaol, *ibo, lightSourceShader);
            pointLights.setLightPosition(1, glm::vec3(3.0f, test, -8.0f));
            renderMesh(*vaol, *ibo, lightSourceShader);
            pointLights.setLightPosition(2, glm::vec3(3.0f, -2.0f, test));
            renderMesh(*vaol, *ibo, lightSourceShader);

            glfwSwapBuffers(m_Window);
            glfwPollEvents();

            k += 0.5f;
            if (k >= 360.0f) k -= 360.0f;
        }

        delete vao, ibo, vaol;

        glfwTerminate();
    }
};

void glfwResizeCallback(GLFWwindow* window, int width, int height);
GLFWwindow* initializeWindow(int width, int height);

int main() {
    const int windowWidth = 1600, windowHeight = 900;

    GLFWwindow* window = initializeWindow(windowWidth, windowHeight);

    Game game(window, windowWidth, windowHeight); 

    glfwSetWindowUserPointer(window, &game);
    glfwSetFramebufferSizeCallback(window, glfwResizeCallback);

    game.run();

    return 0;
}


//CALLBACKS AND UTIL

GLFWwindow* initializeWindow(int width, int height) {
    if (!glfwInit()) {
        std::cout << "GLFW initialization failed!\n"; 
        __debugbreak(); 
    }

    glfwWindowHint(GLFW_SAMPLES, 8); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
     
    GLFWwindow* window = glfwCreateWindow(1600, 900, "Hello World", NULL, NULL); 

    if (!window) {
        glfwTerminate(); 
        std::cout << "GLFW window initialization failed!\n";
        __debugbreak(); 
    }

    return window;
}

void glfwResizeCallback(GLFWwindow* window, int width, int height) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    game->onResize(width, height);
}