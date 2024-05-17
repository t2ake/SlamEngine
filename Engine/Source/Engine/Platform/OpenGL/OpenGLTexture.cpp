#include "OpenGLTexture.h"

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLDefines.h"
#include "RenderCore/RenderCore.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

namespace sl
{

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height, bool mipmap, TextureFormat format, uint32_t flags, const void *pData) :
	m_width(width), m_height(height), m_hasMip(mipmap)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_handle);
	glTextureStorage2D(m_handle, 1, GLInternalTextureFormat[(size_t)format], m_width, m_height);
	if (pData)
	{
		glTextureSubImage2D(m_handle, 0, 0, 0, m_width, m_height, GLTextureFormat[(size_t)format], GL_UNSIGNED_BYTE, pData);
	}

	// Wraps
	glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, GLTextureWrap[(flags & SL_SAMPLER_U_MASK) >> SL_SAMPLER_U_SHIFT]);
	glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, GLTextureWrap[(flags & SL_SAMPLER_V_MASK) >> SL_SAMPLER_V_SHIFT]);
	if ((flags & SL_SAMPLER_BORDER) == SL_SAMPLER_BORDER)
	{
		// TODO: Parameterlize it.
		float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glTextureParameterfv(m_handle, GL_TEXTURE_BORDER_COLOR, borderColor);
	}

	// Filters
	if (mipmap)
	{
		glGenerateTextureMipmap(m_handle);
		glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GLTextureMipmapFilter[(flags & SL_SAMPLER_MIPMAP_MASK) >> SL_SAMPLER_MIPMAP_SHIFT]);
	}
	else
	{
		glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GLTextureFilter[(flags & SL_SAMPLER_MIN_MASK) >> SL_SAMPLER_MIN_SHIFT]);
	}
	glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GLTextureFilter[(flags & SL_SAMPLER_MAG_MASK) >> SL_SAMPLER_MAG_SHIFT]);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &m_handle);
}

void OpenGLTexture2D::Bind(uint32_t slot) const
{
	glBindTextureUnit(slot, m_handle);
}

} // namespace sl
