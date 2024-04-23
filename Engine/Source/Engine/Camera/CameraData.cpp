#include "CameraData.h"

#include <glm/gtc/matrix_transform.hpp>

namespace sl
{

const glm::mat4 &CameraData::GetView()
{
	CheckDirty();

	return m_viewMat;
}

const glm::mat4 &CameraData::GetProjection()
{
	CheckDirty();

	return m_projectionMat;
}

const glm::mat4 &CameraData::GetViewProjection()
{
	CheckDirty();

	return m_viewProjectionMat;
}

void CameraData::RecalculateDir()
{
	m_frontDir.x = glm::cos(m_rotation.y) * glm::cos(m_rotation.x);
	m_frontDir.y = glm::sin(m_rotation.x);
	m_frontDir.z = glm::sin(m_rotation.y) * glm::cos(m_rotation.x);
	m_frontDir = glm::normalize(m_frontDir);

	m_rightDir = glm::normalize(glm::cross(m_frontDir, WorldUp));
	m_upDir = glm::normalize(glm::cross(m_rightDir, m_frontDir));

	m_dirDirty = false;
}

void CameraData::RecalculateMat()
{
	// TODO: Orthogonal camera

	m_viewMat = glm::lookAt(m_position, m_position + m_frontDir, m_upDir);
	m_projectionMat = glm::perspective(glm::radians(m_fov), m_aspect, m_nearPlane, m_farPlane);
	m_viewProjectionMat = m_projectionMat * m_viewMat;

	m_matDirty = false;
}

void CameraData::CheckDirty()
{
	if (m_dirDirty)
	{
		RecalculateDir();
	}
	if (m_matDirty)
	{
		RecalculateMat();
	}
}

} // namespace sl
