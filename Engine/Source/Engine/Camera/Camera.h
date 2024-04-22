#pragma once

#include "Camera/CameraData.h"

#include <glm/vec2.hpp>

namespace sl
{

class Camera final
{
public:
	Camera() = default;

	void Update();

	CameraData &GetData() { return m_data; }
	const CameraData &GetData() const { return m_data; }

	void SetMoveSpeed(float speed) { m_moveSpeed = speed; }
	float &GetMoveSpeed() { return m_moveSpeed; }
	float GetMoveSpeed() const { return m_moveSpeed; }

	void SetRotateSpeed(float speed) { m_rotateSpeed = speed; }
	float &GetRotateSpeed() { return m_rotateSpeed; }
	float GetRotateSpeed() const { return m_rotateSpeed; }

private:
	void UpdateEditorCamera();
	void UpdateFPSCamera();

	CameraData m_data;

	float m_moveSpeed = 0.05f;
	float m_rotateSpeed = 0.005f;
	glm::vec2 m_mousePrePos;

	// TODO: Postprocessing stuff
};

} // namespace sl
