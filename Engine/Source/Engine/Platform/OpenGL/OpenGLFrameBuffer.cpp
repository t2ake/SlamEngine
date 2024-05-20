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
	SL_ENGINE_ASSERT_INFO(!textures.empty(), "Can not create framebuffer without a texture!");

	bool hasColor = false;
	bool isDifferent = false;

	uint32_t minWidth = textures[0]->GetHeight();
	uint32_t minHeight = textures[0]->GetHeight();
	uint32_t colorAttachmentIndex = 0;
	m_attachments.reserve(textures.size());

	for (auto *pTexture : textures)
	{
		AttachmentType type = GetAttachmentType(pTexture->GetFormat());
		GLenum attachmentPoint = GLAttachmentPoint[(size_t)type];

		if (AttachmentType::Color == type)
		{
			hasColor = true;
			attachmentPoint += colorAttachmentIndex++;

			SL_ENGINE_ASSERT_INFO(attachmentPoint < GL_COLOR_ATTACHMENT0 + RenderCore::GetMaxFramebufferColorAttachmentCount(),
				"Color attachments count exceeds the limit!");
		}
		SL_ENGINE_ASSERT_INFO(hasColor, "Can not create framebuffer without a color attachment!");

		m_attachments.emplace_back(pTexture, attachmentPoint);

		uint32_t textureWidth = pTexture->GetWidth();
		uint32_t textureHeight = pTexture->GetHeight();
		if (minWidth != textureWidth || minHeight != textureHeight)
		{
			isDifferent = true;
			minWidth = std::min(minWidth, textureWidth);
			minHeight = std::min(minHeight, textureHeight);
		}
	}
	if (isDifferent)
	{
		SL_ENGINE_WARN("Creating framebuffer with textures of different sizes, shrink to the minimal one.");
	}

	m_width = minWidth;
	m_height = minHeight;
	Create();
}

OpenGLFrameBuffer::~OpenGLFrameBuffer()
{
	glDeleteFramebuffers(1, &m_handle);
	for (auto &attachment : m_attachments)
	{
		delete attachment.pTexture;
	}
	m_attachments.clear();
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
	if (width <= 0 || height <= 0 || width > RenderCore::GetMaxFramebufferSize() || height > RenderCore::GetMaxFramebufferSize())
	{
		SL_ENGINE_ERROR("Invalid frame bufferr size!");
		return;
	}

	if (width == m_width && height == m_height)
	{
		return;
	}

	for (auto &attachment : m_attachments)
	{
		attachment.pTexture->Resize(width, height);
	}

	glDeleteFramebuffers(1, &m_handle);
	m_handle = 0;

	m_width = width;
	m_height = height;
	Create();
}

uint32_t OpenGLFrameBuffer::GetColorAttachmentHandle(size_t i) const
{
	return m_attachments.at(i).pTexture->GetHandle();
}

void OpenGLFrameBuffer::Create()
{
	if (m_handle)
	{
		return;
	}

	glCreateFramebuffers(1, &m_handle);
	glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

	for (const auto &attachment : m_attachments)
	{
		// TODO: Parameterise the 3 and last parameter.
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment.point, GL_TEXTURE_2D, attachment.pTexture->GetHandle(), 0);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		SL_ENGINE_ERROR("Incomplete frame buffer!");
	}
}

} // namespace sl
