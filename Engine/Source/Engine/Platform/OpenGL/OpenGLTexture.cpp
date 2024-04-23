#include "OpenGLTexture.h"

#include "RenderCore/RenderCore.h"
#include "Core//Log.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

namespace sl
{

OpenGLTexture2D::OpenGLTexture2D(std::string path) :
	m_path(std::move(path))
{
	// The first pixel should be the bottom left.
	stbi_set_flip_vertically_on_load(true);

	int width, height, chanel;
	auto *pData = stbi_load(m_path.c_str(), &width, &height, &chanel, 0);
#ifndef SL_FINAL
	if (!pData) { SL_ENGINE_ERROR("Failed to load image {}", m_path); }
#endif
	m_width = (uint32_t)width;
	m_height = (uint32_t)height;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_handle);
	glTextureStorage2D(m_handle, 1, GL_RGB8, m_width, m_height);

	glTextureParameteri(m_handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureSubImage2D(m_handle, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, pData);

	stbi_image_free(pData);
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
