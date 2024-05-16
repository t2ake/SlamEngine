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

#define SL_SAMPLER_NONE                   UINT32_C(0x00000000)
							              
#define SL_SAMPLER_U_CLAMP                UINT32_C(0x00000001)
#define SL_SAMPLER_U_BORDER               UINT32_C(0x00000002)
#define SL_SAMPLER_U_MIRROR               UINT32_C(0x00000003)
#define SL_SAMPLER_U_REPEAT               UINT32_C(0x00000004)
#define SL_SAMPLER_U_MASK                 UINT32_C(0x0000000f)
#define SL_SAMPLER_U_SHIFT                0
							              
#define SL_SAMPLER_V_CLAMP                UINT32_C(0x00000010)
#define SL_SAMPLER_V_BORDER               UINT32_C(0x00000020)
#define SL_SAMPLER_V_MIRROR               UINT32_C(0x00000030)
#define SL_SAMPLER_V_REPEAT               UINT32_C(0x00000040)
#define SL_SAMPLER_V_MASK                 UINT32_C(0x000000f0)
#define SL_SAMPLER_V_SHIFT                4
							              
#define SL_SAMPLER_W_CLAMP                UINT32_C(0x00000100)
#define SL_SAMPLER_W_BORDER               UINT32_C(0x00000200)
#define SL_SAMPLER_W_MIRROR               UINT32_C(0x00000300)
#define SL_SAMPLER_W_REPEAT               UINT32_C(0x00000400)
#define SL_SAMPLER_W_MASK                 UINT32_C(0x00000f00)
#define SL_SAMPLER_W_SHIFT                8
							              
#define SL_SAMPLER_MIN_NEAREST            UINT32_C(0x00001000)
#define SL_SAMPLER_MIN_LINEAR             UINT32_C(0x00002000)
#define SL_SAMPLER_MIN_MASK               UINT32_C(0x00003000)
#define SL_SAMPLER_MIN_SHIFT              12
							              
#define SL_SAMPLER_MAG_NEAREST            UINT32_C(0x00004000)
#define SL_SAMPLER_MAG_LINEAR             UINT32_C(0x00008000)
#define SL_SAMPLER_MAG_MASK               UINT32_C(0x0000c000)
#define SL_SAMPLER_MAG_SHIFT              14
										  
#define SL_SAMPLER_MIPMAP_NEAREST_NEAREST UINT32_C(0x00010000) // Nearest between mipmaps, nearest between pixels.
#define SL_SAMPLER_MIPMAP_LINEAR_NEAREST  UINT32_C(0x00020000) // Linear between mipmaps, nearest between pixels.
#define SL_SAMPLER_MIPMAP_NEAREST_LINEAR  UINT32_C(0x00030000) // Nearest between mipmaps, linear between pixels.
#define SL_SAMPLER_MIPMAP_LINEAR_LINEAR   UINT32_C(0x00040000) // Linear between mipmaps, linear between pixels.
#define SL_SAMPLER_MIPMAP_MASK            UINT32_C(0x000f0000)
#define SL_SAMPLER_MIPMAP_SHIFT           16

#define SL_SAMPLER_UVW_CLAMP (0 \
	| SL_SAMPLER_U_CLAMP \
	| SL_SAMPLER_V_CLAMP \
	| SL_SAMPLER_W_CLAMP \
	)

#define SL_SAMPLER_UVW_BORDER (0 \
	| SL_SAMPLER_U_BORDER \
	| SL_SAMPLER_V_BORDER \
	| SL_SAMPLER_W_BORDER \
	)

#define SL_SAMPLER_UVW_MIRROR (0 \
	| SL_SAMPLER_U_MIRROR \
	| SL_SAMPLER_V_MIRROR \
	| SL_SAMPLER_W_MIRROR \
	)

#define SL_SAMPLER_UVW_REPEAT (0 \
	| SL_SAMPLER_U_REPEAT \
	| SL_SAMPLER_V_REPEAT \
	| SL_SAMPLER_W_REPEAT \
	)

} // namespace sl
