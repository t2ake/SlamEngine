#include "OpenGLTexture.h"

#include "RenderCore/RenderCore.h"
#include "Core//Log.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

namespace sl
{

namespace
{

static constexpr GLint TextureFilter[] =
{
	0,
	GL_NEAREST,
	GL_LINEAR,
};

static constexpr GLint TextureMipmapFilter[] =
{
	0,
	GL_NEAREST_MIPMAP_NEAREST,
	GL_NEAREST_MIPMAP_LINEAR,
	GL_LINEAR_MIPMAP_NEAREST,
	GL_LINEAR_MIPMAP_LINEAR,
};

static constexpr GLint TextureWrap[] =
{
	0,
	GL_CLAMP_TO_EDGE,
	GL_CLAMP_TO_BORDER,
	GL_MIRRORED_REPEAT,
	GL_REPEAT,
	// GL_MIRROR_CLAMP_TO_EDGE,
};

}

OpenGLTexture2D::OpenGLTexture2D(std::string path, bool mipmap, uint32_t flags) :
	m_path(std::move(path))
{
	// The first pixel should at the bottom left.
	stbi_set_flip_vertically_on_load(true);

	int width, height, channels;
	auto *pData = stbi_load(m_path.c_str(), &width, &height, &channels, 0);
	if (!pData)
	{
		SL_ENGINE_ERROR("Failed to load image {}", m_path);
		return;
	}

	m_width = (uint32_t)width;
	m_height = (uint32_t)height;

	GLenum internalFormat = 0;
	GLenum format = 0;
	if (3 == channels)
	{
		internalFormat = GL_RGB8;
		format = GL_RGB;
	}
	else if (4 == channels)
	{
		internalFormat = GL_RGBA8;
		format = GL_RGBA;
	}
	else
	{
		SL_ENGINE_ERROR("Unknown image format of {}", m_path);
		stbi_image_free(pData);
		return;
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &m_handle);
	glTextureStorage2D(m_handle, 1, internalFormat, m_width, m_height);
	glTextureSubImage2D(m_handle, 0, 0, 0, m_width, m_height, format, GL_UNSIGNED_BYTE, pData);
	stbi_image_free(pData);
	
	// Wraps
	glTextureParameteri(m_handle, GL_TEXTURE_WRAP_S, TextureWrap[(flags & SL_SAMPLER_U_MASK) >> SL_SAMPLER_U_SHIFT]);
	glTextureParameteri(m_handle, GL_TEXTURE_WRAP_T, TextureWrap[(flags & SL_SAMPLER_V_MASK) >> SL_SAMPLER_V_SHIFT]);
	if ((flags & SL_SAMPLER_UVW_BORDER) == SL_SAMPLER_UVW_BORDER)
	{
		// TODO: Parameterlize it.
		float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glTextureParameterfv(m_handle, GL_TEXTURE_BORDER_COLOR, borderColor);
	}

	// Filters
	if (mipmap)
	{
		glGenerateTextureMipmap(m_handle);
		glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, TextureMipmapFilter[(flags & SL_SAMPLER_MIPMAP_MASK) >> SL_SAMPLER_MIPMAP_SHIFT]);
	}
	else
	{
		glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, TextureFilter[(flags & SL_SAMPLER_MIN_MASK) >> SL_SAMPLER_MIN_SHIFT]);
	}
	glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, TextureFilter[(flags & SL_SAMPLER_MAG_MASK) >> SL_SAMPLER_MAG_SHIFT]);
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
