#include "OpenGLFrameBuffer.h"

#include "Core/Log.h"

#include <glad/glad.h>

namespace sl
{

OpenGLFrameBuffer::OpenGLFrameBuffer(uint32_t width, uint32_t height) :
	m_width(width), m_height(height)
{
	Create();
}

OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
	glDeleteFramebuffers(1, &m_handle);
	glDeleteTextures(1, &m_color);
	glDeleteTextures(1, &m_depth);
}

void OpenGLFrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
	glViewport(0, 0, m_width, m_height);
}

void OpenGLFrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
{
	if (width <= 0 || height <= 0 || width > m_maxSize || height > m_maxSize)
	{
		SL_ENGINE_ERROR("Invalid frame bufferr size!");
		return;
	}

	if (width == m_width && height == m_height)
	{
		return;
	}

	glDeleteFramebuffers(1, &m_handle);
	glDeleteTextures(1, &m_color);
	glDeleteTextures(1, &m_depth);
	m_handle = m_color = m_depth = 0;

	m_width = width;
	m_height = height;
	Create();
}

void OpenGLFrameBuffer::Create()
{
	if (m_handle)
	{
		return;
	}

	glCreateFramebuffers(1, &m_handle);
	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_color);
	glBindTexture(GL_TEXTURE_2D, m_color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_color, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &m_depth);
	glBindTexture(GL_TEXTURE_2D, m_depth);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_width, m_height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0);

	SL_ENGINE_ASSERT_INFO(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer isn't complete");
}

} // namespace sl
