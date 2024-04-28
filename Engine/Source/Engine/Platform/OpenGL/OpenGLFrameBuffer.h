#pragma once

#include "RenderCore/FrameBuffer.h"

namespace sl
{

class OpenGLFrameBuffer : public FrameBuffer
{
public:
	OpenGLFrameBuffer(uint32_t width, uint32_t height);
	~OpenGLFrameBuffer();

	virtual uint32_t GetWidth() const override { return m_width; }
	virtual uint32_t GetHeight() const override { return m_height; }

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void Resize(uint32_t width, uint32_t height) override;

	virtual uint32_t GetColorAttachmentHandle() const override { return m_color; }

private:
	void Create();

	uint32_t m_handle = 0;
	uint32_t m_color = 0;
	uint32_t m_depth = 0;

	uint32_t m_width = 0;
	uint32_t m_height = 0;
};

} // namespace sl
