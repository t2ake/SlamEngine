#pragma once

#include "RenderCore/IndexBuffer.h"

namespace sl
{

class OpenGLIndexBuffer : public IndexBuffer
{
public:
	OpenGLIndexBuffer(const uint32_t *pIndices, size_t size);
	virtual ~OpenGLIndexBuffer() override;

	virtual void Bind() const override;
	virtual void Unbind() const override;
	virtual uint32_t GetCount() const override { return m_count; };

private:
	uint32_t m_handle = 0;
	uint32_t m_count;
};

} // namespace sl
