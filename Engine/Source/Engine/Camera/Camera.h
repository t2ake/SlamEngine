#pragma once

#include "Camera/CameraData.h"
#include "Event/MouseEvent.h"
#include "Event/WindowEvent.h"
#include "Window/Window.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace sl
{

class Camera final
{
public:
	Camera() = default;

	void Update(float deltaTime);

	CameraData &GetData() { return m_data; }
	const CameraData &GetData() const { return m_data; }

	void SetWindow(Window *pWindow) { m_pWindow = pWindow; }

	void OnEvent(Event &event);

	float GetMoveSpeed() const { return m_basicMoveSpeed; }
	float GetRotateSpeed() const { return m_basicRotateSpeed; }

private:
	void UpdateFPSCamera(float deltaTime);
	void UpdateEditorCamera(float deltaTime);

	bool OnMouseScrolled(MouseScrolledEvent &event);
	bool OnWindowResized(WindowResizeEvent &event);

	bool m_isUpdating = false;
	bool m_isMoving = false;

	CameraData m_data;
	Window *m_pWindow = nullptr;

	float m_basicMoveSpeed = 0.0f;
	const float m_basicRotateSpeed = 0.125f;

	float m_acceleration;
	const float m_defaultAcceleration = 0.375f;
	const float m_maxMoveSpeed = 16.0f;
	const float m_minMoveSpeed = 0.0f;

	float m_moveSpeedMouseScrollMultiplier = 1.0f;
	const float m_moveSpeedKeyShiftMultiplier = 3.0f;

	glm::vec2 m_mousePrePos{ 640.0f, 360.0f };
	glm::vec3 m_lastMovement{ 1.0f, 0.0f, 0.0f };

	// TODO: Postprocessing stuff
};

} // namespace sl
