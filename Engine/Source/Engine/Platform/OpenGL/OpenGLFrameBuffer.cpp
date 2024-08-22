#include "OpenGLFrameBuffer.h"

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLDefines.h"
#include "RenderCore/RenderCore.h"
#include "RenderCore/Texture.h"

#include <glad/glad.h>

namespace sl
{

OpenGLFrameBuffer::OpenGLFrameBuffer(std::vector<Texture2D *> textures, bool destroy) :
	m_destroyTextureWithFramebuffer(destroy)
{
	SL_ASSERT(!textures.empty(), "Can not create framebuffer without any attachments!");

	uint32_t minWidth = textures[0]->GetWidth();
	uint32_t minHeight = textures[0]->GetHeight();
	uint32_t colorAttachmentIndex = 0;
	m_attachments.reserve(textures.size());

	for (auto *pTexture : textures)
	{
		AttachmentType type = GetAttachmentType(pTexture->GetFormat());
		GLenum attachmentPoint = GLAttachmentPoint[(size_t)type];

		if (type == AttachmentType::Color)
		{
			// GL_COLOR_ATTACHMENT0 += index
			attachmentPoint += colorAttachmentIndex++;
			SL_ASSERT(attachmentPoint < GL_COLOR_ATTACHMENT0 + RenderCore::GetInfo().m_maxFramebufferColorAttachmentCount);
		}

		m_attachments.emplace_back(pTexture, attachmentPoint);

		uint32_t textureWidth = pTexture->GetWidth();
		uint32_t textureHeight = pTexture->GetHeight();
		if (minWidth != textureWidth || minHeight != textureHeight)
		{
			minWidth = std::min(minWidth, textureWidth);
			minHeight = std::min(minHeight, textureHeight);

			SL_LOG_WARN("Creating framebuffer with textures of different sizes, shrink to the minimal one: ({}, {})", minWidth, minHeight);
		}
	}
	SL_ASSERT(colorAttachmentIndex, "Can not create framebuffer without any color attachments!");

	m_width = minWidth;
	m_height = minHeight;
	m_colorAttachmentCount = colorAttachmentIndex;

	Create();
}

OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
	if (m_destroyTextureWithFramebuffer)
	{
		for (auto &attachment : m_attachments)
		{
			delete attachment.m_pTexture;
		}
	}
	
	glDeleteFramebuffers(1, &m_handle);
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
#if !defined(SL_FINAL)
	if (width <= 0 || height <= 0 ||
		width > RenderCore::GetInfo().m_maxFramebufferSize ||
		height > RenderCore::GetInfo().m_maxFramebufferSize)
	{
		SL_LOG_ERROR("Invalid frame bufferr size!");
		return;
	}

	if (m_width == width && m_height == height)
	{
		return;
	}
#endif

	for (auto &attachment : m_attachments)
	{
		attachment.m_pTexture->Resize(width, height);
	}

	glDeleteFramebuffers(1, &m_handle);
	m_handle = 0;

	m_width = width;
	m_height = height;

	Create();
}

void OpenGLFrameBuffer::Clear(uint32_t attachmentIndex, const void *pClearData) const
{
	SL_ASSERT(attachmentIndex < m_attachments.size(), "Attachment index out of range!");

	m_attachments.at(attachmentIndex).m_pTexture->Clear(pClearData);
}

int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, uint32_t x, uint32_t y)
{
	SL_ASSERT(attachmentIndex < m_colorAttachmentCount, "Attachment index out of range!");

	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);

	int data;
	TextureFormat format = m_attachments.at(attachmentIndex).m_pTexture->GetFormat();
	// We assume that the origin of the texture is on the upper left,
	// but the origin of OpenGL texture is on the lower left.
	glReadPixels(x, m_height - y, 1, 1, GLTextureFormat[(size_t)format], GLDataType[(size_t)format], &data);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return data;
}

uint32_t OpenGLFrameBuffer::GetAttachmentHandle(size_t i) const
{
	return m_attachments.at(i).m_pTexture->GetHandle();
}

void OpenGLFrameBuffer::Create()
{
#if !defined(SL_FINAL)
	if (m_handle)
	{
		return;
	}
#endif

	glCreateFramebuffers(1, &m_handle);
	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

	for (const auto &attachment : m_attachments)
	{
		// TODO: Parameterise the 3rd and the last parameter.
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment.m_point, GL_TEXTURE_2D, attachment.m_pTexture->GetHandle(), 0);
	}

	// MRT
	if (m_colorAttachmentCount > 1)
	{
		std::vector<GLenum> buffers;
		buffers.reserve(m_colorAttachmentCount);
		for (uint32_t i = 0; i < m_colorAttachmentCount; ++i)
		{
			buffers.emplace_back(GL_COLOR_ATTACHMENT0 + i);
		}

		glNamedFramebufferDrawBuffers(m_handle, m_colorAttachmentCount, buffers.data());
	}

#if !defined(SL_FINAL)
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		SL_LOG_ERROR("Incomplete frame buffer!");
	}
#endif
}

} // namespace sl
