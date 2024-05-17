#pragma once

#include "RenderCore/FrameBuffer.h"

namespace sl
{

class OpenGLFrameBuffer : public FrameBuffer
{
public:
	OpenGLFrameBuffer(uint32_t width, uint32_t height);
	~OpenGLFrameBuffer();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void Resize(uint32_t width, uint32_t height) override;

	virtual void SetMaxSize(uint32_t size) override { m_maxSize = size; }
	virtual uint32_t GetMaxSize() const override { return m_maxSize; }
	virtual uint32_t GetWidth() const override { return m_width; }
	virtual uint32_t GetHeight() const override { return m_height; }

	virtual uint32_t GetColorAttachmentHandle() const override { return m_color; }

private:
	void Create();

	// TODO: 4k for now, should be queried from the GPU driver rather than hard coded here.
	uint32_t m_maxSize = 4096;
	uint32_t m_width = 0;
	uint32_t m_height = 0;

	uint32_t m_color = 0;
	uint32_t m_depth = 0;
	uint32_t m_handle = 0;
};

} // namespace sl
