#pragma once
#include "../rendering/Shader.hpp"
#include "../scene/Camera.hpp"
#include <glm/glm.hpp>

class FlashLight {
public:
	static FlashLight& getFlashLight();

	void setShaderAndCamera(Shader* shader, Camera* camera);
	void setFlashLight(glm::vec3 color, float innerCutOff, float outerCutOff, float brightess);

	void update();
private:
	FlashLight();

	static FlashLight s_Instance;
	Shader* m_Shader;
	Camera* m_Camera;
};