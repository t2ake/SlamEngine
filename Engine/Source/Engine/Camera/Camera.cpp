#include "Camera.h"

#include "Window/Input.h"

#include <glm/trigonometric.hpp>

namespace sl
{

void Camera::Update()
{
	if (Input::GetInstance().IsKeyPressed(SL_KEY_LEFT_ALT))
	{
		if (Input::GetInstance().IsMouseButtonPressed(SL_MOUSE_BUTTON_1))
		{
			UpdateEditorCamera();
		}
	}
	else
	{
		if (Input::GetInstance().IsMouseButtonPressed(SL_MOUSE_BUTTON_2))
		{
			UpdateFPSCamera();
		}
	}
	m_mousePrePos = Input::GetInstance().GetMousePos();
}

void Camera::UpdateEditorCamera()
{

}

void Camera::UpdateFPSCamera()
{
	glm::vec2 crtPos = Input::GetInstance().GetMousePos();
	float offsetX = crtPos.x - m_mousePrePos.x;
	float offsetY = m_mousePrePos.y - crtPos.y;

	m_data.GetRotation() += glm::vec3{ offsetY * m_rotateSpeed, offsetX * m_rotateSpeed, 0.0f };
	m_data.GetRotation().x = std::min(m_data.GetRotation().x, glm::radians(89.9f));
	m_data.GetRotation().x = std::max(m_data.GetRotation().x, glm::radians(-89.9f));
	m_data.RecalculateDir();

	float finalSpeed = m_moveSpeed *
		(Input::GetInstance().IsKeyPressed(SL_KEY_LEFT_SHIFT) ? 2.0f : 1.0f);
	if (Input::GetInstance().IsKeyPressed(SL_KEY_W))
	{
		m_data.GetPosition() += m_data.GetFrontDir() * finalSpeed;
	}
	if (Input::GetInstance().IsKeyPressed(SL_KEY_A))
	{
		m_data.GetPosition() -= m_data.GetRightDir() * finalSpeed;
	}
	if (Input::GetInstance().IsKeyPressed(SL_KEY_S))
	{
		m_data.GetPosition() -= m_data.GetFrontDir() * finalSpeed;
	}
	if (Input::GetInstance().IsKeyPressed(SL_KEY_D))
	{
		m_data.GetPosition() += m_data.GetRightDir() * finalSpeed;
	}
	if (Input::GetInstance().IsKeyPressed(SL_KEY_E))
	{
		m_data.GetPosition() += CameraData::WorldUp * finalSpeed;
	}
	if (Input::GetInstance().IsKeyPressed(SL_KEY_Q))
	{
		m_data.GetPosition() -= CameraData::WorldUp * finalSpeed;
	}
	m_data.RecalculateMat();
}

} // namespace sl
