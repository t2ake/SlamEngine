#pragma once

#include "RenderCore/RenderDefines.h"
#include "RenderCore/VertexArray.h"

namespace sl
{

class RenderAPI
{
public:
	static RenderAPI* Create(GraphicsBackend backend);

public:
	virtual void SetClearColor(float r, float g, float b, float a) = 0;
	virtual void SetClearDepth(float depth) = 0;
	virtual void SetClearStencil(int stencil) = 0;
	virtual void Clear(uint8_t attachments) = 0;

	virtual void DrawIndexed(VertexArray *pVertexArray) = 0;
};

} // namespace sl
