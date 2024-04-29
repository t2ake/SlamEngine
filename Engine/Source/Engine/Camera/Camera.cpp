#include "Camera.h"

#include "Core/Log.h"
#include "Event/MouseEvent.h"
#include "Event/SceneViewportEvent.h"
#include "Window/Input.h"
#include "Window/Window.h"

#include <glm/gtc/epsilon.hpp>
#include <glm/trigonometric.hpp>

#include <algorithm>
#include <array>

namespace sl
{

namespace
{

static constexpr std::array<uint32_t, 6> CamraMoveKey =
{
	SL_KEY_W, SL_KEY_A, SL_KEY_S, SL_KEY_D,
	SL_KEY_E, SL_KEY_Q,
};

}

void Camera::Update(float deltaTime)
{
	if (!m_isActive)
	{
		m_moveSpeed = 0.0f;
		return;
	}

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
		if (m_isRotating)
		{
			m_isRotating = false;
		}
	}
}

void Camera::UpdateFPSCamera(float deltaTime)
{
	// TODO: Camera mode controller logic should be move to ImGuiLayer.

	if (!m_isRotating)
	{
		m_mouseLastPos = Input::GetMousePos();
		m_lastMoveDir = m_data.GetFrontDir();
		m_isRotating = true;
	}

	// Rotation
	{
		glm::vec2 crtPos = Input::GetMousePos();
		glm::vec2 offset{ m_mouseLastPos.y - crtPos.y, crtPos.x - m_mouseLastPos.x };
		m_mouseLastPos = crtPos;

		offset *= m_rotateSpeed * deltaTime;
		m_data.GetRotation() += glm::vec3{ offset, 0.0f };
		m_data.GetRotation().x = std::clamp(m_data.GetRotation().x, glm::radians(-89.9f), glm::radians(89.9f));
	
		m_data.Dirty();
	}

	// TODO: It's better to use bit mask and shift to avoid repeated call of Input::IsKeyPressed
	bool isMoving = false;
	for (auto key : CamraMoveKey)
	{
		if (Input::IsKeyPressed(key))
		{
			isMoving = true;
			break;
		}
	}
	
	glm::vec3 finalMoveDir;
	if (isMoving)
	{
		if (!m_isMoving)
		{
			// Start moving
			m_acceleration = m_maxMoveSpeed / 32.0f;
			m_isMoving = true;
		}

		finalMoveDir = glm::vec3{ 0.0f, 0.0f, 0.0f };
		if (Input::IsKeyPressed(SL_KEY_W))
		{
			finalMoveDir += m_data.GetFrontDir();
		}
		if (Input::IsKeyPressed(SL_KEY_A))
		{
			finalMoveDir -= m_data.GetRightDir();
		}
		if (Input::IsKeyPressed(SL_KEY_S))
		{
			finalMoveDir -= m_data.GetFrontDir();
		}
		if (Input::IsKeyPressed(SL_KEY_D))
		{
			finalMoveDir += m_data.GetRightDir();
		}
		if (Input::IsKeyPressed(SL_KEY_E))
		{
			finalMoveDir += CameraData::WorldUp;
		}
		if (Input::IsKeyPressed(SL_KEY_Q))
		{
			finalMoveDir -= CameraData::WorldUp;
		}
		finalMoveDir = glm::normalize(finalMoveDir);

		if (glm::any(glm::isnan(finalMoveDir)))
		{
			// When pressing buttons in the opposite direction at the same time.
			finalMoveDir = glm::vec3{ 0.0f, 0.0f , 0.0f };
		}
		m_lastMoveDir = finalMoveDir;
	}
	else
	{
		if (m_isMoving)
		{
			// Stop moving
			m_acceleration = -m_maxMoveSpeed / 32.0f;
			m_isMoving = false;
		}
		finalMoveDir = m_lastMoveDir;
	}

	m_moveSpeed += m_acceleration;
	m_moveSpeed = std::clamp(m_moveSpeed, 0.0f, m_maxMoveSpeed);
	m_moveSpeedKeyShiftMultiplier = Input::IsKeyPressed(SL_KEY_LEFT_SHIFT) ? 3.0f : 1.0f;
	float finalMoveSpeed = m_moveSpeed * m_moveSpeedKeyShiftMultiplier * m_moveSpeedMouseScrollMultiplier * deltaTime;

	m_data.GetPosition() += finalMoveDir * finalMoveSpeed;
	m_data.Dirty();
}

void Camera::UpdateEditorCamera(float deltaTime)
{
	// TODO
	if (!m_isRotating)
	{
		m_mouseLastPos = Input::GetMousePos();
		m_lastMoveDir = m_data.GetFrontDir();
		m_isRotating = true;
	}
}

void Camera::OnEvent(Event &event)
{
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseScrollEvent>(BIND_EVENT_CALLBACK(Camera::OnMouseScroll));
	dispatcher.Dispatch<SceneViewportResizeEvent>(BIND_EVENT_CALLBACK(Camera::OnSceneViewportResize));
	dispatcher.Dispatch<SceneViewportFocusEvent>(BIND_EVENT_CALLBACK(Camera::OnSceneViewportFocus));
	dispatcher.Dispatch<SceneViewportLostFocusEvent>(BIND_EVENT_CALLBACK(Camera::OnSceneViewportLostFocus));
}

bool Camera::OnMouseScroll(MouseScrollEvent &event)
{
	m_moveSpeedMouseScrollMultiplier += event.GetOffsetY() * 0.2f;
	m_moveSpeedMouseScrollMultiplier = std::clamp(m_moveSpeedMouseScrollMultiplier, 0.2f, 10.0f);

	return true;
}

bool Camera::OnSceneViewportResize(SceneViewportResizeEvent &event)
{
	float aspect = (float)event.GetWidth() / (float)event.GetHeight();
	float fovDegrees = aspect * (45.0f / 16.0f * 9.0f);
	fovDegrees = std::clamp(fovDegrees, 1.0f, 120.0f);

	m_data.SetAspect(aspect);
	m_data.SetFOVDegrees(fovDegrees);
	m_data.Dirty();

	return true;
}

bool Camera::OnSceneViewportFocus(SceneViewportFocusEvent &event)
{
	m_isActive = true;
	return true;
}

bool Camera::OnSceneViewportLostFocus(SceneViewportLostFocusEvent &event)
{
	m_isActive = false;
	return true;
}

} // namespace sl
