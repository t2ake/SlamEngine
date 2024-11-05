#pragma once

#include "RenderCore/RenderAPI.h"

namespace sl
{

class OpenGLRenderAPI : public RenderAPI
{
public:
	virtual ~OpenGLRenderAPI() = default;

	virtual BackendInfo GetBackendInfo() override;

	virtual void SetClearColor(float r, float g, float b, float a) override;
	virtual void SetClearDepth(float depth) override;
	virtual void SetClearStencil(int  stencil) override;
	virtual void ClearColor() override;
	virtual void ClearDepth() override;
	virtual void ClearStencil() override;

	virtual void SetTextureSlot(uint32_t slot, uint32_t handle) override;

	virtual void EnableDepthTest() override;
	virtual void SetDepthBufferWriteable(bool writeable) override;
	virtual void SetDepthFunc(CompareFunction func) override;

	virtual void EnableBlend() override;
	virtual void SetBlendFunc(BlendFunction sfactor, BlendFunction dfactor) override;
	virtual void SetBlendColor(float r, float g, float b, float a) override;

	virtual void EnableStencil() override;
	virtual void SetStencilMask(uint32_t mask) override;
	virtual void SetStencilFunc(CompareFunction func, int32_t ref, int32_t mask) override;
	virtual void SetStencilOp(StencilOperation fail, StencilOperation zFail, StencilOperation zPass) override;

	virtual void EnableCullFace() override;
	virtual void DisableCullFace() override;
	virtual void SetCullFace(CullingMode mode) override;
	virtual void SetFrontFace(FrontFace face) override;

	virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

	virtual void DrawIndexed(uint32_t count) override;
};

} // namespace sl
