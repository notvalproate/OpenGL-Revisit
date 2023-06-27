#include "OpenGLApp.hpp"

#include "util/ErrorHandling.hpp"
#include "util/Timer.hpp"

#include "rendering/Shader.hpp"
#include "rendering/Buffers.hpp"
#include "rendering/VertexArray.hpp"
#include "rendering/VertexLayout.hpp"

#include "textures/Texture2D.hpp"

#include "scene/Camera.hpp"
#include "scene/CameraHandler.hpp"

#include "lighting/PointLight.hpp"
#include "lighting/DirectionalLight.hpp"
#include "lighting/FlashLight.hpp"

#include "modeling/Mesh.hpp"
#include "modeling/Model.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class App : public OpenGLApp {
public:
    void run() override {
        Shader globalShader(L"src/shaders/global/vertex.shader", L"src/shaders/global/fragment.shader");
        VertexLayout globalLayout;
        globalLayout.push(VertexAttribute::Position);
        globalLayout.push(VertexAttribute::Normal);
        globalLayout.push(VertexAttribute::TextureCoordinates);
        globalLayout.push(VertexAttribute::TextureIndex);
        globalShader.setLayout(globalLayout);


        Shader lightSourceShader(L"src/shaders/light_source/vertex.shader", L"src/shaders/light_source/fragment.shader");
        VertexLayout lightSourceLayout;
        lightSourceLayout.push(VertexAttribute::Position);
        lightSourceShader.setLayout(lightSourceLayout);

        Timer timer;
        Camera camera(m_WindowData.width, m_WindowData.height);
        CameraHandler camHandler(camera);

        //LIGHTING

        DirectionalLight& directionalLight = DirectionalLight::getDirectionalLight();
        directionalLight.setShader(&globalShader);

        PointLightList& pointLights = PointLightList::getList();
        pointLights.setShader(&globalShader);

        FlashLight& flashLight = FlashLight::getFlashLight();
        flashLight.setShaderAndCamera(&globalShader, &camera);

        {
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

        //MODELS

        Model backpack(L"assets/models/backpack/backpack.obj", &globalShader);

        //MAIN GAME LOOP

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

            //Render backpack
            backpack.draw();

            //Render the lights
            float test = 8.0f * glm::sin(glm::radians(k));
            float test2 = 8.0f * glm::sin(glm::radians(k + 90.0f));

            pointLights.setLightPosition(0, glm::vec3(test2, 0.0f, 4.0f));

            pointLights.setLightPosition(1, glm::vec3(3.0f, test2, -8.0f));

            pointLights.setLightPosition(2, glm::vec3(3.0f, -2.0f, test));

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