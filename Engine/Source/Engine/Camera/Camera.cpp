#include "Camera.h"

#include "Core/Log.h"
#include "Window/Input.h"

#include <glm/trigonometric.hpp>

namespace sl
{

namespace
{

static std::array<uint32_t, 6> CamraMoveKey =
{
	SL_KEY_W, SL_KEY_A, SL_KEY_S, SL_KEY_D,
	SL_KEY_E, SL_KEY_Q,
};

}

void Camera::Update(float deltaTime)
{
	// TODO: Check is mouse in scene view.

	// Camera FPS mode.
	if (Input::IsMouseButtonPressed(SL_MOUSE_BUTTON_2))
	{
		UpdateFPSCamera(deltaTime);
	}
	// Camera editor mode.
	else if (Input::IsKeyPressed(SL_KEY_LEFT_ALT) && Input::IsMouseButtonPressed(SL_MOUSE_BUTTON_1))
	{
		UpdateEditorCamera(deltaTime);
	}
	// Camera unused.
	else
	{
		if (m_isUpdating)
		{
			m_pWindow->EnableCursor();
			m_isUpdating = false;
		}
	}
}

void Camera::UpdateFPSCamera(float deltaTime)
{
	if (!m_isUpdating)
	{
		m_mousePrePos = Input::GetMousePos();
		m_pWindow->DisableCursor();
		m_isUpdating = true;
	}

	// Rotation
	{
		glm::vec2 crtPos = Input::GetMousePos();
		float offsetX = crtPos.x - m_mousePrePos.x;
		float offsetY = m_mousePrePos.y - crtPos.y;
		m_mousePrePos = Input::GetMousePos();

		m_data.GetRotation() += glm::vec3{ offsetY * m_basicRotateSpeed * deltaTime, offsetX * m_basicRotateSpeed * deltaTime, 0.0f };
		m_data.GetRotation().x = std::min(m_data.GetRotation().x, glm::radians(89.9f));
		m_data.GetRotation().x = std::max(m_data.GetRotation().x, glm::radians(-89.9f));
	}

	// TODO: The camera with acceleration is implemented roughly,
	// need to check and improve in the future.

	glm::vec3 finalMovement{ 0.0f, 0.0f, 0.0f };

	bool isMoving = false;
	for (auto key : CamraMoveKey)
	{
		isMoving |= Input::IsKeyPressed(key);
	}
	if (isMoving)
	{
		if (!m_isMoving)
		{
			m_acceleration = m_defaultAcceleration;
			m_isMoving = true;
		}
	}
	else
	{
		finalMovement = m_lastMovement;
		if (m_isMoving)
		{
			m_acceleration = -m_defaultAcceleration;
			m_isMoving = false;
		}
	}

	m_basicMoveSpeed += m_acceleration;
	m_basicMoveSpeed = std::max(m_minMoveSpeed, m_basicMoveSpeed);
	m_basicMoveSpeed = std::min(m_maxMoveSpeed, m_basicMoveSpeed);

	float finalMoveSpeed = deltaTime * m_basicMoveSpeed * m_moveSpeedMouseScrollMultiplier *
		(Input::IsKeyPressed(SL_KEY_LEFT_SHIFT) ? m_moveSpeedKeyShiftMultiplier : 1.0f);

	if (Input::IsKeyPressed(SL_KEY_W))
	{
		finalMovement += m_data.GetFrontDir();
	}
	if (Input::IsKeyPressed(SL_KEY_A))
	{
		finalMovement -= m_data.GetRightDir();
	}
	if (Input::IsKeyPressed(SL_KEY_S))
	{
		finalMovement -= m_data.GetFrontDir();
	}
	if (Input::IsKeyPressed(SL_KEY_D))
	{
		finalMovement += m_data.GetRightDir();
	}
	if (Input::IsKeyPressed(SL_KEY_E))
	{
		finalMovement += CameraData::WorldUp;
	}
	if (Input::IsKeyPressed(SL_KEY_Q))
	{
		finalMovement -= CameraData::WorldUp;
	}
	finalMovement = glm::normalize(finalMovement);

	m_data.GetPosition() += finalMoveSpeed * finalMovement;
	m_lastMovement = finalMovement;

	m_data.Dirty();
}

void Camera::UpdateEditorCamera(float deltaTime)
{
	// TODO
	if (!m_isUpdating)
	{
		m_mousePrePos = Input::GetMousePos();
		m_pWindow->DisableCursor();
		m_isUpdating = true;
	}
}

void Camera::OnEvent(Event &event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_CALLBACK(Camera::OnMouseScrolled));
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_CALLBACK(Camera::OnWindowResized));
}

bool Camera::OnMouseScrolled(MouseScrolledEvent &event)
{
	m_moveSpeedMouseScrollMultiplier += event.GetOffsetY() * 0.2f;
	m_moveSpeedMouseScrollMultiplier = std::max(m_minMoveSpeed, m_moveSpeedMouseScrollMultiplier);
	m_moveSpeedMouseScrollMultiplier = std::min(m_maxMoveSpeed, m_moveSpeedMouseScrollMultiplier);

	return true;
}

bool Camera::OnWindowResized(WindowResizeEvent &event)
{
	m_data.SetAspect((float)event.GetWidth() / (float)event.GetHeight());
	m_data.Dirty();
	return true;
}


} // namespace sl
