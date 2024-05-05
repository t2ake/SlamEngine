#pragma once

#include <glm/vec3.hpp>

namespace sl
{

struct TransformComponent
{
	TransformComponent() = default;
	TransformComponent(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
		m_position(position), m_rotation(rotation), m_scale(scale) {}

	glm::vec3 m_position{ 0.0f, 0.0f , 0.0f };
	// Stores in radians
	glm::vec3 m_rotation{ 0.0f, 0.0f , 0.0f };
	glm::vec3 m_scale{ 1.0f, 1.0f , 1.0f };
};

} // namespace sl
