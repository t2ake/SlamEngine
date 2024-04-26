#include "RenderCore.h"

#include "Core/Log.h"

namespace sl
{

GraphicsBackend RenderCore::m_backend = GraphicsBackend::None;
RenderAPI *RenderCore::m_pRenderAPI = nullptr;

void RenderCore::Init(GraphicsBackend backend)
{
	SL_ENGINE_ASSERT(!m_pRenderAPI);

	m_backend = backend;
	m_pRenderAPI = RenderAPI::Create();
}

void RenderCore::SetClearColor(float r, float g, float b, float a)
{
	m_pRenderAPI->SetClearColor(r, g, b, a);
}

void RenderCore::SetClearDepth(float depth)
{
	m_pRenderAPI->SetClearDepth(depth);
}

void RenderCore::SetClearStencil(int stencil)
{
	m_pRenderAPI->SetClearStencil(stencil);
}

void RenderCore::Clear(uint8_t attachments)
{
	m_pRenderAPI->Clear(attachments);
}

void RenderCore::OnMainViewportResize(uint32_t width, uint32_t height)
{
	m_pRenderAPI->SetViewport(0, 0, width, height);
}

void RenderCore::SetDefaultState()
{
	m_pRenderAPI->SetDefaultState();
}

void RenderCore::Submit(VertexArray *pVertexArray, Shader *pShader)
{
	pVertexArray->Bind();
	pShader->Bind();
	m_pRenderAPI->DrawIndexed(pVertexArray->GetIndexBuffer()->GetCount());
}

} // namespace sl
