#include "OpenGLApp.hpp"

#include "util/Timer.hpp"

#include "rendering/Shader.hpp"

#include "scene/CameraHandler.hpp"

#include "lighting/PointLight.hpp"
#include "lighting/DirectionalLight.hpp"
#include "lighting/FlashLight.hpp"

#include "modeling/ModelLoader.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

class App : public OpenGLApp {
public:
    void run() override {
        Timer timer;
        Camera camera(m_WindowData.width, m_WindowData.height);
        CameraHandler camHandler(camera);

        //SHADER SETUP

        Shader globalShader(L"src/shaders/global/vertex.shader", L"src/shaders/global/fragment.shader"); 
        VertexLayout globalLayout(
            { VertexAttribute::Position, VertexAttribute::Normal, VertexAttribute::TextureCoordinates, VertexAttribute::MaterialIndex }
        );
        globalShader.setLayout(globalLayout); 

        Shader bufferShader(L"src/shaders/framebuffershader/vertex.shader", L"src/shaders/framebuffershader/fragment.shader");
        VertexLayout bufferShaderLayout(
            { VertexAttribute::Position, VertexAttribute::TextureCoordinates }
        );
        bufferShader.setLayout(bufferShaderLayout);

        //MODELS

        ModelLoader modelLoader;

        Model backpack = modelLoader.loadModel(L"assets/models/backpack/backpack.obj", &globalShader, true);

        Model cottage = modelLoader.loadModel(L"assets/models/cottage/Cottage_FREE.obj", &globalShader, false);
        cottage.setModelMatrix(glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, -5.0f, 0.0f)), glm::vec3(4.0f)));

        Model aya = modelLoader.loadModel(L"assets/models/aya/091_W_Aya_100K.obj", &globalShader, false);
        aya.setModelMatrix(glm::translate(glm::rotate(glm::scale(glm::mat4(1.0f), glm::vec3(0.007f)), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(-1550.0f, -160.0f, -800.0f)));

        Model agera = modelLoader.loadModel(L"assets/models/agera/agera.obj", &globalShader, true);
        agera.setModelMatrix(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.98f)), glm::vec3(-12.0f, -5.0f, 0.0f)));

        Model madhav = modelLoader.loadModel(L"assets/models/bike/Mountain_Bike.obj", &globalShader, false);
        madhav.setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -4.0f, 44.0f)));

        //DIRECTIONAL LIGHT SETUP

        DirectionalLight& directionalLight = DirectionalLight::getDirectionalLight();
        directionalLight.setShader(&globalShader);
        directionalLight.setDirectionalLight(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f), 1.0f);


        //POINTLIGHT SETUP

        PointLightList& pointLights = PointLightList::getList();
        pointLights.setShader(&globalShader);

        glm::vec3 lightPos(3.0f, 0.0f, 4.0f);
        glm::vec3 color(1.0f, 1.0f, 1.0f); 

        pointLights.addLight(0, lightPos, color, 1.0f); 

        color = glm::vec3(1.0f, 0.1f, 0.1f); 
        pointLights.addLight(1, lightPos, color, 1.0f); 

        color = glm::vec3(0.1f, 1.0f, 0.0f); 
        pointLights.addLight(2, lightPos, color, 1.0f); 


        //SPOTLIGHT SETUP

        FlashLight& flashLight = FlashLight::getFlashLight();
        flashLight.setShaderAndCamera(&globalShader, &camera);
        flashLight.setFlashLight(glm::vec3(1.0f, 0.9f, 0.9f), 12.5f, 17.5f, 1.0f);

        //FRAME BUFFERS
        unsigned int fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, m_WindowData.width, m_WindowData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_WindowData.width, m_WindowData.height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);


        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        

        //TRIANGLE
        std::vector<float> vquad = {
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f
        };

        std::vector<unsigned int> vindices = {
            0, 1, 2,
            2, 3, 0
        };

        VertexBuffer vbo(vquad);
        VertexArray vao(&vbo, bufferShader.getLayout());
        IndexBuffer ibo(vindices);

        //MAIN GAME LOOP
        float k = 0.5f;
        while (!glfwWindowShouldClose(m_Window)) {
            if (m_WindowData.resized) camera.resetViewport(m_WindowData.width, m_WindowData.height);
            float deltaTime = timer.getDeltaTime();

            float test = 8.0f * glm::sin(glm::radians(k));
            float test2 = 8.0f * glm::sin(glm::radians(k + 90.0f));

            //CAMERA UPDATES
            camHandler.handleEvents(m_Window, deltaTime);
            camera.updateUniforms("u_View", "u_Projection", "u_ViewPos", globalShader);

            //Move and update the lights 
            pointLights.setLightPosition(0, glm::vec3(test2, 0.0f, 4.0f));
            pointLights.setLightPosition(1, glm::vec3(3.0f, test2, -8.0f));
            pointLights.setLightPosition(2, glm::vec3(3.0f, -2.0f, test));

            flashLight.update();

            glBindFramebuffer(GL_FRAMEBUFFER, fbo);

            glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);

            //Render backpack
            madhav.draw();
            backpack.draw();
            agera.draw();
            cottage.draw();
            aya.draw();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glDisable(GL_DEPTH_TEST);

            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_2D, texture);
            bufferShader.setUniform1i("tex", 1);

            bufferShader.bind();
            vao.bind();
            ibo.bind();
            GLCall(glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_INT, nullptr));

            glfwSwapBuffers(m_Window);
            glfwPollEvents();
            
            k += 120.0f * deltaTime;
            if (k >= 360.0f) k -= 360.0f;
        }

        //glDeleteFramebuffers(1, &fbo);

        glfwTerminate();
    }
};

int main() {
    App app; 

    try {
        app.initialize(1600, 900);
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}