#pragma once

// Do not include this file in any headers.

#include "Core/Defines.h"
#include "Core/EnumOf.hpp"

#include <glad/glad.h>

namespace sl
{

inline constexpr GLenum GLAttribType[nameof::enum_count<AttribType>()] =
{
	GL_BYTE,           // AttribType::Int8
	GL_UNSIGNED_BYTE,  // AttribType::Uint8
	GL_SHORT,          // AttribType::Int16
	GL_UNSIGNED_SHORT, // AttribType::Uint16
	GL_INT,            // AttribType::Int32
	GL_UNSIGNED_INT,   // AttribType::Uint32
	GL_HALF_FLOAT,     // AttribType::Half
	GL_FLOAT,          // AttribType::Float
	GL_DOUBLE,         // AttribType::Double
};

inline constexpr GLenum GLShaderType[nameof::enum_count<ShaderType>()] =
{
	GL_VERTEX_SHADER,   // ShaderType::VertexShader
	GL_FRAGMENT_SHADER, // ShaderType::FragmentShader
	GL_COMPUTE_SHADER,  // ShaderType::ComputeShader
};

inline constexpr GLenum GLCompareFunc[nameof::enum_count<CompareFunction>()] =
{
	GL_NEVER,	 // DepthFunc::Never 
	GL_LESS,	 // DepthFunc::Less
	GL_LEQUAL,	 // DepthFunc::LessEqual
	GL_GREATER,	 // DepthFunc::Greater
	GL_GEQUAL,	 // DepthFunc::GreaterEqual
	GL_EQUAL,    // DepthFunc::Equal
	GL_NOTEQUAL, // DepthFunc::NotEqual
	GL_ALWAYS,	 // DepthFunc::Always
};

inline constexpr GLenum GLStencilOp[nameof::enum_count<StencilOperation>()] =
{
	GL_KEEP,	  // StencilOp::Keep
	GL_ZERO,	  // StencilOp::Zero
	GL_REPLACE,	  // StencilOp::Replace
	GL_INCR,	  // StencilOp::Increase
	GL_INCR_WRAP, // StencilOp::IncreaseWrap
	GL_DECR,	  // StencilOp::Decrease
	GL_DECR_WRAP, // StencilOp::DecreaseWrap
	GL_INVERT,	  // StencilOp::Invert
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

} // namespace sl
