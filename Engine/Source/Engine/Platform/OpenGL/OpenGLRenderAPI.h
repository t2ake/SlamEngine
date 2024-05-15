#pragma once

#include "RenderCore/RenderAPI.h"

namespace sl
{

class OpenGLRenderAPI : public RenderAPI
{
public:
	virtual ~OpenGLRenderAPI() = default;

	virtual void SetClearColor(float r, float g, float b, float a) override;
	virtual void SetClearDepth(float depth) override;
	virtual void SetClearStencil(int  stencil) override;
	virtual void ClearColor() override;
	virtual void ClearDepth() override;
	virtual void ClearStencil() override;

	virtual void EnableBlend() override;

	virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

	virtual void DrawIndexed(uint32_t count) override;
};

} // namespace sl
