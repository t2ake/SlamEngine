#pragma once

#include "RenderCore/FrameBuffer.h"
#include "RenderCore/RenderAPI.h"
#include "RenderCore/Shader.h"
#include "RenderCore/Texture.h"
#include "RenderCore/VertexArray.h"

namespace sl
{

class RenderCore final
{
public:
	RenderCore() = delete;

	static void Init(GraphicsBackend backend);
	static GraphicsBackend GetBackend() { return m_backend; }
	static void SetDefaultState();

	static void SetMainFrameBuffer(FrameBuffer *pBuffer) { m_pMainFrameBuffer = pBuffer; }
	static FrameBuffer *GetMainFrameBuffer() { return m_pMainFrameBuffer; }

	static void SetClearColor(float r, float g, float b, float a);
	static void SetClearDepth(float depth);
	static void SetClearStencil(int stencil);
	static void Clear(uint8_t attachments);

	static void Submit(VertexArray *pVertexArray, Shader *pShader);

private:
	inline static GraphicsBackend m_backend = GraphicsBackend::None;
	inline static RenderAPI *m_pRenderAPI = nullptr;

	inline static FrameBuffer *m_pMainFrameBuffer = nullptr;
};

} // namespace sl
