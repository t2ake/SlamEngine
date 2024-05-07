#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace sl
{

struct TransformComponent
{
	TransformComponent() = default;
	TransformComponent(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
		m_position(position), m_rotation(rotation), m_scale(scale) {}

	glm::vec3 GetRotationDegrees() const { return glm::degrees(m_rotation); }
	void SetRotationDegrees(glm::vec3 degrees) { m_rotation = glm::radians(std::move(degrees)); }

	glm::mat4 GetTransform() const
	{
		glm::mat4 ratation =
			glm::rotate(glm::mat4{ 1.0f }, m_rotation.x, { 1.0f, 0.0f, 0.0f }) *
			glm::rotate(glm::mat4{ 1.0f }, m_rotation.y, { 0.0f, 1.0f, 0.0f }) *
			glm::rotate(glm::mat4{ 1.0f }, m_rotation.z, { 0.0f, 0.0f, 1.0f });

		return glm::translate(glm::mat4{ 1.0f }, m_position) *
			ratation *
			glm::scale(glm::mat4{ 1.0f }, m_scale);
	}

	glm::vec3 m_position{ 0.0f, 0.0f , 0.0f };
	// Stores in radians
	glm::vec3 m_rotation{ 0.0f, 0.0f , 0.0f };
	glm::vec3 m_scale{ 1.0f, 1.0f , 1.0f };
};

} // namespace sl
