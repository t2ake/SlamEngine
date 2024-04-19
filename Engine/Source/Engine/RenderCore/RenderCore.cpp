#include "RenderCore.h"

#include "Log/Log.h"

namespace sl
{

void RenderCore::SetBackend(GraphicsBackend backend)
{
	SL_ENGINE_ASSERT(!m_pRenderAPI);

	m_backend = backend;
	m_pRenderAPI = RenderAPI::Create(backend);
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

void RenderCore::Submit(VertexArray *pVertexArray, Shader *pShader)
{
	pShader->Bind();
	pVertexArray->Bind();
	m_pRenderAPI->DrawIndexed(pVertexArray);
	// pShader->Unbind();
	// pVertexArray->Unbind();
}

} // namespace sl
