#pragma once

#include "RenderCore/VertexBuffer.h"

namespace sl
{

class OpenGLVertexBuffer : public VertexBuffer
{
public:
	OpenGLVertexBuffer(const float *pVertices, size_t size);
	virtual ~OpenGLVertexBuffer() override;

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void SetLayout(VertexLayout layout) override { m_layout = std::move(layout); }
	virtual VertexLayout &GetLayout() override { return m_layout; }
	virtual const VertexLayout &GetLayout() const override { return m_layout; }

private:
	uint32_t m_handle = 0;
	VertexLayout m_layout;
};

} // namespace sl
