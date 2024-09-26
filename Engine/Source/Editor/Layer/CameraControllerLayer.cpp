#include "CameraControllerLayer.h"

#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"
#include "Event/SceneViewportEvent.h"
#include "Scene/ECSWorld.h"
#include "Utils/ProfilerCPU.h"
#include "Window/Input.h"
#include "Window/Window.h"

#include <algorithm>
#include <array>

void CameraControllerLayer::OnAttach()
{

}

void CameraControllerLayer::OnDetach()
{

}

void CameraControllerLayer::OnEvent(sl::Event &event)
{
	SL_PROFILE;

	sl::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<sl::MouseScrollEvent>(BIND_EVENT_CALLBACK(CameraControllerLayer::OnMouseScroll));
	dispatcher.Dispatch<sl::SceneViewportResizeEvent>(BIND_EVENT_CALLBACK(CameraControllerLayer::OnSceneViewportResize));
	dispatcher.Dispatch<sl::MouseButtonPressEvent>(BIND_EVENT_CALLBACK(CameraControllerLayer::OnMouseButtonPress));
	dispatcher.Dispatch<sl::MouseButtonReleaseEvent>(BIND_EVENT_CALLBACK(CameraControllerLayer::OnMouseButtonRelease));
}

void CameraControllerLayer::BeginFrame()
{

}

void CameraControllerLayer::OnUpdate(float deltaTime)
{
	SL_PROFILE;

	const auto &mode = sl::ECSWorld::GetMainCameraComponent().m_controllerMode;

	if (mode == sl::CameraControllerMode::None)
	{
		m_isRotating = false;
		m_isMoving = false;
		return;
	}

	if (mode == sl::CameraControllerMode::FPS)
	{
		UpdateFPSMode(deltaTime);
	}
	else if (mode == sl::CameraControllerMode::Editor)
	{
		UpdateEditorMode(deltaTime);
	}
}

void CameraControllerLayer::OnRender()
{

}

void CameraControllerLayer::EndFrame()
{

}

void CameraControllerLayer::UpdateFPSMode(float deltaTime)
{
	auto [camera, transform] = sl::ECSWorld::GetMainCameraEntity().GetComponents<sl::CameraComponent, sl::TransformComponent>();

	if (!m_isRotating)
	{
		// When the camera starts to rotate.
		camera.m_acceleration = 0.0f;
		camera.m_moveSpeed = 0.0f;
		m_isRotating = true;
		sl::Input::GetMouseDelta();
	}

	// Rotation
	{
		glm::vec2 offset = sl::Input::GetMouseDelta();
		transform.m_rotation += glm::vec3{ glm::vec2{ -offset.y, offset.x } * camera.m_rotateSpeed, 0.0f };
		transform.m_rotation.x = std::clamp(transform.m_rotation.x, glm::radians(-89.9f), glm::radians(89.9f));

		camera.m_isDirty = true;
	}

	constexpr std::array<uint32_t, 6> CamraMoveKey =
	{
		SL_KEY_W, SL_KEY_A, SL_KEY_S, SL_KEY_D, SL_KEY_Q, SL_KEY_E,
	};

	uint8_t moveKeyMask = 0x00;
	for (size_t i = 0; i < CamraMoveKey.size(); ++i)
	{
		if (sl::Input::IsKeyPressed(CamraMoveKey[i]))
		{
			moveKeyMask |= UINT8_C(1 << i);
		}
	}

	glm::vec3 finalMoveDir;
	if (moveKeyMask)
	{
		if (!m_isMoving)
		{
			// When the camera starts to move.
			camera.m_acceleration = camera.m_maxMoveSpeed * camera.m_maxSpeedToAcceleration;
			m_isMoving = true;
		}

		finalMoveDir = glm::vec3{ 0.0f, 0.0f, 0.0f };
		const std::array<glm::vec3, 6> moveBehaviors =
		{
			camera.m_frontDir,             // SL_KEY_W
			-camera.m_rightDir,            // SL_KEY_A
			-camera.m_frontDir,            // SL_KEY_S
			camera.m_rightDir,             // SL_KEY_D
			-sl::CameraComponent::WorldUp, // SL_KEY_Q
			sl::CameraComponent::WorldUp,  // SL_KEY_E
		};
		for (size_t i = 0; i < CamraMoveKey.size(); ++i)
		{
			if (moveKeyMask & UINT8_C(1 << i))
			{
				finalMoveDir += moveBehaviors[i];
			}
		}
		finalMoveDir = glm::normalize(finalMoveDir);

		if (glm::any(glm::isnan(finalMoveDir)))
		{
			// When pressing buttons in the opposite direction at the same time.
			finalMoveDir = glm::vec3{ 0.0f, 0.0f, 0.0f };
		}

		camera.m_lastMoveDir = finalMoveDir;
	}
	else
	{
		if (m_isMoving)
		{
			// Stop moving
			camera.m_acceleration = -camera.m_maxMoveSpeed * camera.m_maxSpeedToAcceleration;
			m_isMoving = false;
		}
		finalMoveDir = camera.m_lastMoveDir;
	}

	camera.m_moveSpeed += camera.m_acceleration * deltaTime;
	camera.m_moveSpeed = std::clamp(camera.m_moveSpeed, 0.0f, camera.m_maxMoveSpeed);
	float moveSpeedKeyShiftMultiplier = sl::Input::IsKeyPressed(SL_KEY_LSHIFT) ? camera.m_moveSpeedKeyShiftMultiplier : 1.0f;
	float finalMoveSpeed = camera.m_moveSpeed * moveSpeedKeyShiftMultiplier * camera.m_moveSpeedMouseScrollMultiplier * deltaTime;

	transform.m_position += finalMoveDir * finalMoveSpeed;
	camera.m_isDirty = true;
}

void CameraControllerLayer::UpdateEditorMode(float deltaTime)
{
	 // TODO
}

bool CameraControllerLayer::OnMouseScroll(sl::MouseScrollEvent &event)
{
	auto &camera = sl::ECSWorld::GetMainCameraComponent();
	if (camera.m_controllerMode == sl::CameraControllerMode::None)
	{
		return false;
	}

	camera.m_moveSpeedMouseScrollMultiplier += event.GetOffsetY() * 0.1f;
	camera.m_moveSpeedMouseScrollMultiplier = std::clamp(camera.m_moveSpeedMouseScrollMultiplier, 0.1f, 10.0f);

	return true;
}

bool CameraControllerLayer::OnSceneViewportResize(sl::SceneViewportResizeEvent &event)
{
	float aspect = (float)event.GetWidth() / (float)event.GetHeight();
	float fovMultiplier = aspect * 9.0f / 16.0f;

	auto &camera = sl::ECSWorld::GetMainCameraComponent();
	camera.m_aspect = aspect;
	camera.m_fovMultiplier = fovMultiplier;
	camera.m_isDirty = true;

	return false;
}

bool CameraControllerLayer::OnMouseButtonPress(sl::MouseButtonPressEvent &event)
{
	const auto &mode = sl::ECSWorld::GetMainCameraComponent().m_controllerMode;

	if (mode == sl::CameraControllerMode::FPS || mode == sl::CameraControllerMode::Editor)
	{
		sl::Window::GetInstance().SetMouseRelativeMode(true);
	}

	return false;
}

bool CameraControllerLayer::OnMouseButtonRelease(sl::MouseButtonReleaseEvent &event)
{
	auto &mode = sl::ECSWorld::GetMainCameraComponent().m_controllerMode;

	if ((mode == sl::CameraControllerMode::FPS && event.GetButton() == SL_MOUSE_BUTTON_RIGHT) ||
		(mode == sl::CameraControllerMode::Editor&& event.GetButton() == SL_MOUSE_BUTTON_LEFT))
	{
		mode = sl::CameraControllerMode::None;
		sl::Window::GetInstance().SetMouseRelativeMode(false);
	}

	return false;
}

bool CameraControllerLayer::OnKeyRelease(sl::KeyReleaseEvent &event)
{
	auto &mode = sl::ECSWorld::GetMainCameraComponent().m_controllerMode;

	if (mode == sl::CameraControllerMode::Editor && event.GetKey() == SL_KEY_LALT)
	{
		mode = sl::CameraControllerMode::None;
	}

	return false;
}
