#pragma once

#include "RenderCore/VertexBuffer.h"
#include "RenderCore/IndexBuffer.h"

namespace sl
{

class VertexArray
{
public:
	static VertexArray *Create();

public:
	virtual ~VertexArray() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void SetVertexBuffer(VertexBuffer *pVertexBuffer) = 0;
	virtual void SetIndexBuffer(IndexBuffer *pIndexBuffer) = 0;

	virtual VertexBuffer *GetVertexBuffer() = 0;
	virtual IndexBuffer *GetIndexBuffer() = 0;
};

} // namespace sl
