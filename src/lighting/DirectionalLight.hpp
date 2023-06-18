#pragma once
#include "../rendering/Shader.hpp"
#include <glm/glm.hpp>

class DirectionalLight {
public:
	static DirectionalLight& getDirectionalLight();

	void setShader(Shader* shader);
	void setDirectionalLight(glm::vec3 direction, glm::vec3 color, float brightness);
private:
	DirectionalLight();

	static DirectionalLight s_Instance;
	Shader* m_Shader;
};