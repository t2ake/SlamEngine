#pragma once

#include "Core/Defines.h"
#include "RenderCore/BackendInfo.h"

namespace sl
{

class RenderAPI
{
public:
	static RenderAPI *Create();

public:
	virtual ~RenderAPI() = default;

	virtual BackendInfo GetBackendInfo() = 0;

	virtual void SetClearColor(float r, float g, float b, float a) = 0;
	virtual void SetClearDepth(float depth) = 0;
	virtual void SetClearStencil(int stencil) = 0;
	virtual void ClearColor() = 0;
	virtual void ClearDepth() = 0;
	virtual void ClearStencil() = 0;

	virtual void SetTextureSlot(uint32_t slot, uint32_t handle) = 0;

	virtual void EnableDepthTest() = 0;
	virtual void SetDepthBufferWriteable(bool writeable) = 0;
	virtual void SetDepthFunc(CompareFunction func) = 0;

	virtual void EnableBlend() = 0;
	virtual void SetBlendFunc(BlendFunction sfactor, BlendFunction dfactor) = 0;
	virtual void SetBlendColor(float r, float g, float b, float a) = 0;

	virtual void EnableStencil() = 0;
	virtual void SetStencilMask(uint32_t mask) = 0;
	virtual void SetStencilFunc(CompareFunction func, int32_t ref, int32_t mask) = 0;
	virtual void SetStencilOp(StencilOperation fail, StencilOperation zFail, StencilOperation zPass) = 0;

	virtual void EnableCullFace() = 0;
	virtual void DisableCullFace() = 0;
	virtual void SetCullFace(CullingMode mode) = 0;
	virtual void SetFrontFace(FrontFace face) = 0;

	virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

	virtual void DrawIndexed(uint32_t count) = 0;
};

} // namespace sl
