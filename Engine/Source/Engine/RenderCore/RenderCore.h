#pragma once

#include "RenderCore/RenderAPI.h"
#include "RenderCore/Shader.h"
#include "RenderCore/VertexArray.h"

namespace sl
{

class RenderCore final
{
public:
	RenderCore() = delete;

	static void Init(GraphicsBackend backend);

	static GraphicsBackend GetBackend() { return m_backend; }
	
	static void SetClearColor(float r, float g, float b, float a);
	static void SetClearDepth(float depth);
	static void SetClearStencil(int stencil);
	static void Clear(uint8_t attachments);

	static void OnMainViewportResize(uint32_t width, uint32_t height);

	// TEMPORARY
	static void DefaultBlend();

	static void Submit(VertexArray *pVertexArray, Shader *pShader);

private:
	static GraphicsBackend m_backend;
	static RenderAPI *m_pRenderAPI;
};

} // namespace sl
