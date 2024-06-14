#pragma once

#include "RenderCore/VertexArray.h"

#include <memory>

namespace sl
{

class OpenGLVertexArray : public VertexArray
{
public:
	OpenGLVertexArray();
	virtual ~OpenGLVertexArray() override;

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void SetVertexBuffer(VertexBuffer *pVertexBuffer) override;
	virtual void SetIndexBuffer(IndexBuffer *pIndexBuffer) override;

	virtual VertexBuffer *GetVertexBuffer() override { return m_pVertexBuffer.get(); }
	virtual IndexBuffer *GetIndexBuffer() override { return m_pIndexBuffer.get(); }

private:
	std::unique_ptr<VertexBuffer> m_pVertexBuffer;
	std::unique_ptr<IndexBuffer> m_pIndexBuffer;

	uint32_t m_handle = 0;
};

} // namespace sl
