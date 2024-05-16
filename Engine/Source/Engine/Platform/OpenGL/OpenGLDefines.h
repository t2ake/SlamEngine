#pragma once

// Do not include this file in any headers.

#include "Core/Defines.h"
#include "Core/EnumOf.hpp"

#include <glad/glad.h>

namespace sl
{

inline constexpr GLint GLDepthFunc[nameof::enum_count<DepthFunc>()] =
{
	GL_NEVER,
	GL_LESS,
	GL_EQUAL,
	GL_LEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_GEQUAL,
	GL_ALWAYS,
};

inline constexpr GLint GLTextureFilter[] =
{
	0,
	GL_NEAREST,
	GL_LINEAR,
};

inline constexpr GLint GLTextureMipmapFilter[] =
{
	0,
	GL_NEAREST_MIPMAP_NEAREST,
	GL_NEAREST_MIPMAP_LINEAR,
	GL_LINEAR_MIPMAP_NEAREST,
	GL_LINEAR_MIPMAP_LINEAR,
};

inline constexpr GLint GLTextureWrap[] =
{
	0,
	GL_CLAMP_TO_EDGE,
	GL_CLAMP_TO_BORDER,
	GL_MIRRORED_REPEAT,
	GL_REPEAT,
	// GL_MIRROR_CLAMP_TO_EDGE,
};

}
