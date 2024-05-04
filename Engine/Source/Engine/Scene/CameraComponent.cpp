#include "CameraComponent.h"

#include "Scene/ECSWorld.h"

#include <glm/gtc/matrix_transform.hpp>

namespace sl
{

const glm::mat4 &CameraComponent::GetView()
{
	Recalculate();
	return m_viewMat;
}

const glm::mat4 &CameraComponent::GetProjection()
{
	Recalculate();
	return m_projectionMat;
}

const glm::mat4 &CameraComponent::GetViewProjection()
{
	Recalculate();
	return m_viewProjectionMat;
}

void CameraComponent::Recalculate()
{
	if (!m_isDirty)
	{
		return;
	}

	auto transform = ECSWorld::GetMainCameraEntity().GetComponent<TransformComponent>();
	auto rotation = transform.m_rotation;
	auto position = transform.m_position;

	m_frontDir.x = glm::cos(rotation.y) * glm::cos(rotation.x);
	m_frontDir.y = glm::sin(rotation.x);
	m_frontDir.z = glm::sin(rotation.y) * glm::cos(rotation.x);
	m_frontDir = glm::normalize(m_frontDir);

	m_rightDir = glm::normalize(glm::cross(m_frontDir, WorldUp));
	m_upDir = glm::normalize(glm::cross(m_rightDir, m_frontDir));

	// TODO: Orthogonal camera
	m_viewMat = glm::lookAt(position, position + m_frontDir, m_upDir);
	m_projectionMat = glm::perspective(m_fov, m_aspect, m_nearPlane, m_farPlane);
	m_viewProjectionMat = m_projectionMat * m_viewMat;

	m_isDirty = false;
}

} // nmaespace sl
