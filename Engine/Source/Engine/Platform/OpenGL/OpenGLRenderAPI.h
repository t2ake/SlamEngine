#pragma once

#include "RenderCore/RenderAPI.h"

namespace sl
{

class OpenGLRenderAPI : public RenderAPI
{
public:
	virtual void SetClearColor(float r, float g, float b, float a) override;
	virtual void SetClearDepth(float depth) override;
	virtual void SetClearStencil(int  stencil) override;
	virtual void Clear(uint8_t attachments) override;

	virtual void DrawIndexed(VertexArray *pVertexArray) override;
};

} // namespace sl
