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
	virtual void ClearColor() = 0;
	virtual void ClearDepth() = 0;
	virtual void ClearStencil() = 0;

	virtual void EnableDepthTest() = 0;
	virtual void SetDepthBufferWriteable(bool writeable) = 0;
	virtual void SetDepthFunc(DepthFunc func) = 0;

	virtual void EnableBlend() = 0;

	virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

	virtual void DrawIndexed(uint32_t count) = 0;
};

} // namespace sl
