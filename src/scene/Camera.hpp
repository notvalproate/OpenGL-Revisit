#pragma once
#include <glm/glm.hpp>
#include <string_view>
#include "../rendering/Shader.hpp"

class Camera {
public:
	Camera();
	Camera(const float p_Speed, const float p_Sensitivity, const float p_Fov);

	void UpdateUniforms(std::string_view p_View, std::string_view p_Projection, Shader& p_Shader);
	void UpdateUniforms(std::string_view p_View, std::string_view p_Projection, std::string_view p_ViewPos, Shader& p_Shader);
	void ResetProjectionMat(int p_Width, int p_Height);

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