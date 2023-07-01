#pragma once
#include <unordered_map>
#include <string>
#include "../rendering/Shader.hpp"
#include <glm/glm.hpp>

class PointLight {
private:
	PointLight(unsigned short index, const glm::vec3& position, const glm::vec3& color, float brightness, Shader* shader);

	std::string m_Index;

	void resetUniforms(Shader* shader);
	void setPosition(const glm::vec3& position, Shader* shader);

	friend class PointLightList;
};

class PointLightList {
public:
	PointLightList(const PointLightList& other) = delete;

	static PointLightList& getList();

	void addLight(unsigned short Index, const glm::vec3& Position, const glm::vec3& Color, float Brightness);
	void removeLight(unsigned short Index);

	void setShader(Shader* Shader);
	void setLightPosition(unsigned short Index, const glm::vec3& Position);
private:
	PointLightList();

	static PointLightList s_Instance;
	std::unordered_map<unsigned short, PointLight*> m_PointLights;
	Shader* m_Shader;
};