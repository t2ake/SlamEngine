#pragma once

#include "Core/Defines.h"
#include "RenderCore/VertexArray.h"

namespace sl
{

class RenderAPI
{
public:
	static RenderAPI* Create();

public:
	virtual ~RenderAPI() = default;

	virtual void SetClearColor(float r, float g, float b, float a) = 0;
	virtual void SetClearDepth(float depth) = 0;
	virtual void SetClearStencil(int stencil) = 0;
	virtual void Clear(uint8_t attachments) = 0;

	virtual void DefaultBlend() = 0;

	virtual void DrawIndexed(uint32_t count) = 0;
};

} // namespace sl
