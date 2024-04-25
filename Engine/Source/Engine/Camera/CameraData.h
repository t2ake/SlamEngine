#pragma once

#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

namespace sl
{

class CameraData final
{
public:
	static constexpr glm::vec3 WorldUp = { 0.0f, 1.0f, 0.0f };

public:
	CameraData() = default;

	void SetPosition(glm::vec3 position) { m_position = std::move(position); }
	glm::vec3 &GetPosition() { return m_position; }
	const glm::vec3 &GetPosition() const { return m_position; }

	void SetRotation(glm::vec3 rotation) { m_rotation = std::move(rotation); }
	glm::vec3 &GetRotation() { return m_rotation; }
	const glm::vec3 &GetRotation() const { return m_rotation; }

	void SetRotationDegrees(glm::vec3 rotation) { m_rotation = glm::radians(std::move(rotation)); }
	glm::vec3 GetRotationDegrees() const { return glm::degrees(m_rotation); }

	void SetAspect(float aspect) { m_aspect = aspect; }
	float &GetAspect() { return m_aspect; }
	float GetAspect() const { return m_aspect; }

	void SetFOV(float fov) { m_fov = fov; }
	float &GetFOV() { return m_fov; }
	float GetFOV() const { return m_fov; }

	void SetFOVDegrees(float fov) { m_fov = glm::radians(fov); }
	float GetFOVDegrees() { return glm::degrees(m_fov); }

	void SetNearPlane(float nearPlane) { m_nearPlane = nearPlane; }
	float &GetNearPlane() { return m_nearPlane; }
	float GetNearPlane() const { return m_nearPlane; }

	void SetFarPlane(float farPlane) { m_farPlane = farPlane; }
	float &GetFarPlane() { return m_farPlane; }
	float GetFarPlane() const { return m_farPlane; }

	void SetFrontDir(glm::vec3 dir) { m_frontDir = std::move(dir); }
	glm::vec3 &GetFrontDir() { return m_frontDir; }
	const glm::vec3 &GetFrontDir() const { return m_frontDir; }

	void SetUpDir(glm::vec3 dir) { m_upDir = std::move(dir); }
	glm::vec3 &GetUpDir() { return m_upDir; }
	const glm::vec3 &GetUpDir() const { return m_upDir; }

	void SetRightDir(glm::vec3 dir) { m_rightDir = std::move(dir); }
	glm::vec3 &GetRightDir() { return m_rightDir; }
	const glm::vec3 &GetRightDir() const { return m_rightDir; }

	const glm::mat4 &GetView();
	const glm::mat4 &GetProjection();
	const glm::mat4 &GetViewProjection();

	void Dirty() { m_isDirty = true; }
	void Recalculate();

private:
	// TODO: Move to ECS Transform Component
	glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
	// Pitch, Yaw, Roll in radians.
	glm::vec3 m_rotation{ 0.0f, 0.0f, 0.0f };

	// TODO: Move to ECS Camera Component
	float m_aspect = 1920.0f / 1080.0f;
	float m_fov = glm::radians(45.0f);
	float m_nearPlane = 0.01f;
	float m_farPlane = 10000.0f;

	// Cache
	bool m_isDirty = true;

	glm::vec3 m_frontDir{ 0.0f, 0.0f, 1.0f };
	glm::vec3 m_upDir{ 0.0f, 1.0f, 0.0f };
	glm::vec3 m_rightDir{ 1.0f, 0.0f, 0.0f };

	glm::mat4 m_viewMat{ 1.0f };
	glm::mat4 m_projectionMat{ 1.0f };
	glm::mat4 m_viewProjectionMat{ 1.0f };
};

} // namespace sl
