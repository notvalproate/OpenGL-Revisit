#pragma once
#include <glm/glm.hpp>
#include <string_view>
#include "../rendering/Shader.hpp"

class Camera {
public:
	Camera();
	Camera(const int viewportWidth, const int viewportHeight); 
	Camera(const int viewportWidth, const int viewportHeight, const float speed, const float sensitivity, const float fov);

	void updateUniforms(std::string_view view, std::string_view projection, Shader& shader);
	void updateUniforms(std::string_view view, std::string_view projection, std::string_view viewPos, Shader& shader);
	void resetViewport(int width, int height);

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