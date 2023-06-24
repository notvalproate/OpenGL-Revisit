#include "OpenGLApp.hpp"

#include "util/ErrorHandling.hpp"
#include "util/Timer.hpp"

#include "rendering/Shader.hpp"
#include "rendering/Buffers.hpp"
#include "rendering/VertexArray.hpp"

#include "textures/Texture2D.hpp"

#include "scene/Camera.hpp"
#include "scene/CameraHandler.hpp"

#include "lighting/PointLight.hpp"
#include "lighting/DirectionalLight.hpp"
#include "lighting/FlashLight.hpp"

#include "modeling/Mesh.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class App : public OpenGLApp {
public:
    void run() override {
        Shader globalShader(L"src/shaders/global/vertex.shader", L"src/shaders/global/fragment.shader");
        Shader lightSourceShader(L"src/shaders/light_source/vertex.shader", L"src/shaders/light_source/fragment.shader");

        Timer timer;
        Camera camera(m_WindowData.width, m_WindowData.height);
        CameraHandler camHandler(camera);

        //BASIC MESH
        Mesh* boxMesh, *lightMesh;

        //LIGHTING
        DirectionalLight& directionalLight = DirectionalLight::getDirectionalLight();
        directionalLight.setShader(&globalShader);

        PointLightList& pointLights = PointLightList::getList();
        pointLights.setShader(&globalShader);

        FlashLight& flashLight = FlashLight::getFlashLight();
        flashLight.setShaderAndCamera(&globalShader, &camera);

        //Temporary Lambda to render a mesh
        const auto renderMesh = [](const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) {
            vao.bind();
            ibo.bind();
            shader.bind();
            GLCall(glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr));
        };

        //Temporary scope to show how to initialize vao and ibo in a mesh object
        {
            std::vector<unsigned int> layout = { 3, 3, 2, 1 };
            globalShader.setLayout(layout);

            std::vector<unsigned int> lightLayout = { 3 };
            lightSourceShader.setLayout(lightLayout);

            //All data passed in as parameters
            float vertices[] = {
                //POSITION         //NORMALS         //TEXCOORD  //TEX INDEX
                -0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                 0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

                -0.5f,-0.5f,-0.5f, 0.0f, 0.0f,-1.0f, 0.0f, 0.0f, 0.0f,
                 0.5f,-0.5f,-0.5f, 0.0f, 0.0f,-1.0f, 1.0f, 0.0f, 0.0f,
                 0.5f, 0.5f,-0.5f, 0.0f, 0.0f,-1.0f, 1.0f, 1.0f, 0.0f,
                -0.5f, 0.5f,-0.5f, 0.0f, 0.0f,-1.0f, 0.0f, 1.0f, 0.0f,

                 0.5f,-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                 0.5f,-0.5f,-0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 0.5f, 0.5f,-0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                 0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                -0.5f,-0.5f, 0.5f,-1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                -0.5f,-0.5f,-0.5f,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                -0.5f, 0.5f,-0.5f,-1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f,-1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                -0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

                -0.5f,-0.5f, 0.5f, 0.0f,-1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                 0.5f,-0.5f, 0.5f, 0.0f,-1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                 0.5f,-0.5f,-0.5f, 0.0f,-1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
                -0.5f,-0.5f,-0.5f, 0.0f,-1.0f, 0.0f, 0.0f, 1.0f, 0.0f
            };

            unsigned int indices[] = {
                0, 1, 2, 2, 3, 0,
                4, 7, 6, 6, 5, 4,
                8, 9, 10, 10, 11, 8,
                12, 15, 14, 14, 13, 12,
                16, 17, 18, 18, 19, 16,
                20, 23, 22, 22, 21, 20
            };

            std::vector<Texture2D> textures = {
                Texture2D("assets/textures/crate.png", TextureType::DIFFUSE,GL_NEAREST, GL_CLAMP_TO_EDGE),
                Texture2D("assets/textures/crate_spec.png", TextureType::SPECULAR, GL_NEAREST, GL_CLAMP_TO_EDGE)
            };

            boxMesh = new Mesh(vertices, indices, textures, &globalShader);

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

            std::vector<Texture2D> temp {};

            lightMesh = new Mesh(lightVertices, indices, temp, &lightSourceShader);

            //LIGHTING
            flashLight.setFlashLight(glm::vec3(1.0f, 0.9f, 0.9f), 12.5f, 17.5f, 1.0f);

            directionalLight.setDirectionalLight(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.9f, 0.58f, 0.19f), 0.5f);

            glm::vec3 lightPos(3.0f, 0.0f, 4.0f);
            glm::vec3 color(1.0f, 1.0f, 1.0f);

            pointLights.addLight(0, lightPos, color, 1.0f, &lightSourceShader);

            color = glm::vec3(1.0f, 0.1f, 0.1f);
            pointLights.addLight(1, lightPos, color, 1.0f, &lightSourceShader);

            color = glm::vec3(0.1f, 1.0f, 0.0f);
            pointLights.addLight(2, lightPos, color, 1.0f, &lightSourceShader);
        }

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

        Texture2D diffuseMap("assets/textures/crate.png", TextureType::DIFFUSE,GL_NEAREST, GL_CLAMP_TO_EDGE); 
        Texture2D specularMap("assets/textures/crate_spec.png", TextureType::SPECULAR, GL_NEAREST, GL_CLAMP_TO_EDGE); 

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

            //Rendering box at all positions
            float i = 10.0f;
            for (const auto& pos : boxPositions) {
                boxMesh->setModelMatrix(glm::translate(glm::rotate(glm::mat4(1.0f), glm::radians(i), pos), pos));
                boxMesh->draw();
                i += 30.0f;
            }

            //Render the lights
            float test = 8.0f * glm::sin(glm::radians(k));
            float test2 = 8.0f * glm::sin(glm::radians(k + 90.0f));

            pointLights.setLightPosition(0, glm::vec3(test2, 0.0f, 4.0f));
            lightMesh->setModelMatrix(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(test2, 0.0f, 4.0f)), glm::vec3(0.2)));
            lightMesh->draw();

            pointLights.setLightPosition(1, glm::vec3(3.0f, test2, -8.0f));
            lightMesh->setModelMatrix(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, test2, -8.0f)), glm::vec3(0.2)));
            lightMesh->draw(); 

            pointLights.setLightPosition(2, glm::vec3(3.0f, -2.0f, test));           
            lightMesh->setModelMatrix(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, -2.0f, test)), glm::vec3(0.2)));
            lightMesh->draw(); 

             
            flashLight.update();

            glfwSwapBuffers(m_Window);
            glfwPollEvents();
            
            k += 120.0f * deltaTime;
            if (k >= 360.0f) k -= 360.0f;
        }

        glfwTerminate();
    }
};

int main() {
    App app; 

    app.initialize(1600, 900);
    app.run();

    return 0;
}