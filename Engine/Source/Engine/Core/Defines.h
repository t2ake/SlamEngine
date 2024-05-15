#pragma once

#include <cstdint>

namespace sl
{

enum class ProjectionType : uint8_t
{
	Perspective = 0,
	Orthographic,
};

enum class CameraControllerMode : uint8_t
{
	None = 0,
	FPS,
	Editor,
};

enum class GraphicsBackend : uint8_t
{
	None = 0,
	OpenGL,
	DirectX11,
	DirectX12,
	Vulkan,
	Metal,
};

enum class AttribType : uint8_t
{
	Int8 = 0,
	Uint8,
	Int16,
	Uint16,
	Int32,
	Uint32,

	Half,
	Float,
	Double,
};

} // namespace sl
