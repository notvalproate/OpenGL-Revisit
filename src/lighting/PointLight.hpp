#pragma once
#include <unordered_map>
#include <string>
#include "../rendering/Shader.hpp"
#include <glm/glm.hpp>

class PointLight {
private:
	PointLight(unsigned short a_Index, const glm::vec3& a_Position, const glm::vec3& a_Color, float a_Brightness, Shader* a_Shader, Shader* a_ModelShader);

	//Temporary until mesh/model class is implemented
	glm::mat4 m_Model;
	glm::vec3 m_ModelColor;

	Shader* m_ModelShader;
	std::string m_Index;

	void resetUniforms(Shader* a_Shader);
	void setPosition(const glm::vec3& a_Position, Shader* a_Shader);

	friend class PointLightList;
};

class PointLightList {
public:
	PointLightList(const PointLightList& other) = delete;

	static PointLightList& getList() {
		return s_Instance;
	}

	void addLight(unsigned short a_Index, const glm::vec3& a_Position, const glm::vec3& a_Color, float a_Brightness, Shader* a_ModelShader);
	void removeLight(unsigned short a_Index);

	void setShader(Shader* a_Shader);
	void setLightPosition(unsigned short a_Index, const glm::vec3& a_Position);
private:
	PointLightList() : m_PointLights({}), m_Shader(nullptr) {}

	static PointLightList s_Instance;
	std::unordered_map<unsigned short, PointLight*> m_PointLights;
	Shader* m_Shader;
};