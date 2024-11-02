#pragma once

#include "Asset/Shader/Shared/Light.h"
#include "Asset/Shader/Shared/UniformBuffer.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cstdint>

namespace sl
{

enum class LightType : uint32_t
{
	Directional = SL_LIGHT_TYPE_DIRECTIONAL,
	Point = SL_LIGHT_TYPE_POINT,
	Spot = SL_LIGHT_TYPE_SPOT,
};

struct LightComponent
{
	LightType type = LightType::Point;
	glm::vec3 color{ 1.0f };
	float intensity = 1024.0f;
	float range = 1024.0f;
	float outer = glm::radians(45.0f); // Store in radiance
	float inner = glm::radians(30.0f); // Store in radiance
};

// std140
struct LightUniformBuffer
{
	uint32_t type;
	float intensity;
	float range;
	float scale;
	float offset;
	float padding0, padding1, padding2;
	glm::vec4 color;
	glm::vec4 position;
	glm::vec4 direction;
};

} // namespace sl
