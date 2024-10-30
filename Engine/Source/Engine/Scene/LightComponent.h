#pragma once

#include "Asset/Shader/Shared/Light.h"

#include <glm/glm.hpp>

#include <cstdint>

namespace sl
{

enum class LightType : uint32_t
{
	Directional = LIGHT_TYPE_DIRECTIONAL,
	Point = LIGHT_TYPE_POINT,
	Spot = LIGHT_TYPE_SPOT,
};

struct LightComponent
{
	LightType type = LightType::Point;
	glm::vec3 color{ 1.0f };
	float intensity = 1024.0f;
	float range = 1024.0f;
	float outer = glm::radians(45.0f);
	float inner = glm::radians(30.0f);
};

// std140
struct LightUniformBuffer
{
	uint32_t type;
	float intensity;
	float range;
	float outer;
	float inner;
	float padding0, padding1, padding2;
	glm::vec4 color;
	glm::vec4 position;
	glm::vec4 direction;
};

} // namespace sl
