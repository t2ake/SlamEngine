#pragma once

#include "Core/Defines.h"

namespace sl
{

class RenderAPI
{
public:
	static RenderAPI *Create();

public:
	virtual ~RenderAPI() = default;

	virtual void SetClearColor(float r, float g, float b, float a) = 0;
	virtual void SetClearDepth(float depth) = 0;
	virtual void SetClearStencil(int stencil) = 0;
	virtual void Clear(uint8_t attachments) = 0;

	virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

	virtual void SetDefaultState() = 0;

	virtual void DrawIndexed(uint32_t count) = 0;
};

} // namespace sl
