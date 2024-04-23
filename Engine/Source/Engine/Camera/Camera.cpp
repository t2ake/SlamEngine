#include "Camera.h"

#include "Core/Log.h"
#include "Window/Input.h"

#include <glm/trigonometric.hpp>

namespace sl
{

void Camera::Update(float deltaTime)
{
	//TODO: Check is mouse in scene view.

	// Camera FPS mode.
	if (Input::GetInstance().IsMouseButtonPressed(SL_MOUSE_BUTTON_2))
	{
		UpdateFPSCamera(deltaTime);
	}
	// Camera editor mode.
	else if (Input::GetInstance().IsKeyPressed(SL_KEY_LEFT_ALT) &&
		Input::GetInstance().IsMouseButtonPressed(SL_MOUSE_BUTTON_1))
	{
		UpdateEditorCamera(deltaTime);
	}
	// Camera unused.
	else
	{
		if (m_isMoving)
		{
			m_pWindow->EnableCursor();
			m_isMoving = false;
		}
	}
}

void Camera::UpdateEditorCamera(float deltaTime)
{
	if (!m_isMoving)
	{
		m_mousePrePos = Input::GetInstance().GetMousePos();
		m_pWindow->DisableCursor();
		m_isMoving = true;
	}
}

void Camera::UpdateFPSCamera(float deltaTime)
{
	if (!m_isMoving)
	{
		m_mousePrePos = Input::GetInstance().GetMousePos();
		m_pWindow->DisableCursor();
		m_isMoving = true;
	}

	glm::vec2 crtPos = Input::GetInstance().GetMousePos();
	float offsetX = crtPos.x - m_mousePrePos.x;
	float offsetY = m_mousePrePos.y - crtPos.y;
	m_mousePrePos = Input::GetInstance().GetMousePos();

	m_data.GetRotation() += glm::vec3{ offsetY * m_rotateSpeed * deltaTime, offsetX * m_rotateSpeed * deltaTime, 0.0f };
	m_data.GetRotation().x = std::min(m_data.GetRotation().x, glm::radians(89.9f));
	m_data.GetRotation().x = std::max(m_data.GetRotation().x, glm::radians(-89.9f));
	m_data.RecalculateDir();

	float finalSpeed = m_moveSpeed * deltaTime *
		(Input::GetInstance().IsKeyPressed(SL_KEY_LEFT_SHIFT) ? 3.0f : 1.0f);
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
