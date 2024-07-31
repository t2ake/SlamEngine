#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

namespace sl
{

struct TransformComponent
{
	void Reset()
	{
		m_position = glm::vec3{ 0.0f, 0.0f , 0.0f };
		m_rotation = glm::vec3{ 0.0f, 0.0f , 0.0f };
		m_scale = glm::vec3{ 1.0f, 1.0f , 1.0f };
	}

	TransformComponent() = default;
	TransformComponent(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale) :
		m_position(position), m_rotation(rotation), m_scale(scale) {}

	glm::vec3 GetRotationDegrees() const { return glm::degrees(m_rotation); }
	void SetRotationDegrees(const glm::vec3 &degrees) { m_rotation = glm::radians(degrees); }

	glm::mat4 GetTransform() const
	{
		return glm::translate(glm::mat4{ 1.0f }, m_position) *
			glm::toMat4(glm::quat(m_rotation)) *
			glm::scale(glm::mat4{ 1.0f }, m_scale);
	}

	glm::vec3 m_position{ 0.0f, 0.0f , 0.0f };
	glm::vec3 m_rotation{ 0.0f, 0.0f , 0.0f }; // Stores in radians
	glm::vec3 m_scale{ 1.0f, 1.0f , 1.0f };
};

} // namespace sl
