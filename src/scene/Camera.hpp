#pragma once
#include <glm/glm.hpp>
#include <string_view>
#include "../rendering/Shader.hpp"

class Camera {
public:
	Camera();
	Camera(const float a_Speed, const float a_Sensitivity, const float a_Fov);

	void updateUniforms(std::string_view a_View, std::string_view a_Projection, Shader& a_Shader);
	void updateUniforms(std::string_view a_View, std::string_view a_Projection, std::string_view a_ViewPos, Shader& a_Shader);
	void resetViewport(int a_Width, int a_Height);

	[[nodiscard]] glm::vec3 GetPos() const { return m_Position; }
private:
	glm::vec3 m_Position;
	glm::vec3 m_Direction;
	glm::vec3 m_Up;

	glm::mat4 m_View;
	glm::mat4 m_Projection;

	float m_Fov;
	float m_Speed;
	float m_SpeedMultiplier;
	float m_Sensitivity;

	double m_Yaw;
	double m_Pitch;

	friend class CameraHandler;
};