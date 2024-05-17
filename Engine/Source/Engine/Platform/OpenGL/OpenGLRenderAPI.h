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

	virtual void EnableDepthTest() override;
	virtual void DepthBufferWriteable(bool writeable) override;
	virtual void DepthFunc(CompareFunction func) override;

	virtual void EnableBlend() override;
	virtual void BlendFunc(BlendFunction sfactor, BlendFunction dfactor) override;
	virtual void BlendColor(float r, float g, float b, float a) override;

	virtual void EnableStencil() override;
	virtual void StencilMask(uint32_t mask) override;
	virtual void StencilFunc(CompareFunction func, int32_t ref, int32_t mask) override;
	virtual void StencilOp(StencilOperation fail, StencilOperation zFail, StencilOperation zPass) override;

	virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

	virtual void DrawIndexed(uint32_t count) override;
};

} // namespace sl
