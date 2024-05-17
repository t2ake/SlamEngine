#pragma once

#include "RenderCore/VertexArray.h"

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

	virtual VertexBuffer *GetVertexBuffer() override { return m_pVertexBuffer; }
	virtual IndexBuffer *GetIndexBuffer() override { return m_pIndexBuffer; }

private:
	VertexBuffer *m_pVertexBuffer = nullptr;
	IndexBuffer *m_pIndexBuffer = nullptr;

	uint32_t m_handle = 0;
};

} // namespace sl
