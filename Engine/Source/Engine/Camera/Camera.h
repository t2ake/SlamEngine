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

	void SetMaxMoveSpeed(float speed) { m_maxMoveSpeed = speed; }
	float &GetMaxMoveSpeed() { return m_maxMoveSpeed; }
	float GetMaxMoveSpeed() const { return m_maxMoveSpeed; }

private:
	void UpdateFPSCamera(float deltaTime);
	void UpdateEditorCamera(float deltaTime);

	bool OnMouseScrolled(MouseScrolledEvent &event);
	bool OnWindowResized(WindowResizeEvent &event);

	CameraData m_data;
	Window *m_pWindow = nullptr;

	bool m_isActive = false;
	bool m_isMoving = false;

	float m_rotateSpeed = 0.125f;
	float m_acceleration = -16.0f / 50.0f;
	float m_maxMoveSpeed = 16.0f;
	float m_moveSpeed = 0.0f;

	float m_moveSpeedMouseScrollMultiplier = 1.0f; 
	float m_moveSpeedKeyShiftMultiplier = 1.0f; 

	glm::vec2 m_mouseLastPos{ 640.0f, 360.0f };
	glm::vec3 m_lastMoveDir{ 1.0f, 0.0f, 0.0f };

	// TODO: Postprocessing stuff
};

} // namespace sl
