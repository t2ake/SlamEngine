#include "RenderCore.h"

#include "Core/Log.h"

namespace sl
{

void RenderCore::Init()
{
	m_pRenderAPI.reset(RenderAPI::Create());
	m_info = m_pRenderAPI->GetBackendInfo();

	SL_LOG_TRACE("\tMax texture size: {}", m_info.m_maxTextureSize);
	SL_LOG_TRACE("\tMax framebuffer size: {}", m_info.m_maxFramebufferSize);
	SL_LOG_TRACE("\tMax framebuffer color attachment count: {}", m_info.m_maxFramebufferColorAttachmentCount);
	SL_LOG_TRACE("\tMax vertex uniform component count: {}", m_info.m_maxVertexUniformComponentCount);
	SL_LOG_TRACE("\tMax fragment uniform component count: {}", m_info.m_maxFragmentUniformComponentCount);
	SL_LOG_TRACE("\tMax uniform location: {}", m_info.m_maxUniformLocation);
	SL_LOG_TRACE("\tMax uniform buffer binding: {}", m_info.m_maxUniformBufferBinding);
}

void RenderCore::SetUniformBuffer(std::string_view name, UniformBuffer *pUniformBuffer)
{
	if (auto it = m_UniformBuffers.find(name.data()); m_UniformBuffers.end() != it)
	{
		SL_LOG_ERROR("Uniform buffer binding point already exists!");
		return;
	}

	m_UniformBuffers[name.data()].reset(pUniformBuffer);
}

UniformBuffer *RenderCore::GetUniformBuffer(std::string_view name)
{
	auto it = m_UniformBuffers.find(name.data());
	if (it == m_UniformBuffers.end())
	{
		SL_LOG_ERROR("Uniform buffer binding point does not exist!");
		return nullptr;
	}

	return it->second.get();
}

void RenderCore::SetDefaultState()
{
	m_pRenderAPI->EnableDepthTest();
	m_pRenderAPI->SetDepthFunc(CompareFunction::Less);

	m_pRenderAPI->EnableBlend();
	m_pRenderAPI->SetBlendFunc(BlendFunction::SrcAlpha, BlendFunction::OneMinusSrcAlpha);
}

void RenderCore::ClearColor(float r, float g, float b, float a)
{
	m_pRenderAPI->SetClearColor(r, g, b, a);
	m_pRenderAPI->ClearColor();
}

void RenderCore::ClearDepth(float depth)
{
	m_pRenderAPI->SetClearDepth(depth);
	m_pRenderAPI->ClearDepth();
}

void RenderCore::ClearStencil(int  stencil)
{
	m_pRenderAPI->SetClearStencil(stencil);
	m_pRenderAPI->ClearStencil();
}

void RenderCore::Culling(CullingMode mode)
{
	if (mode == CullingMode::FrontNitherBack)
	{
		m_pRenderAPI->DisableCullFace();
		return;
	}

	m_pRenderAPI->EnableCullFace();
	m_pRenderAPI->SetCullFace(mode);
}

void RenderCore::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	m_pRenderAPI->SetViewport(x, y, width, height);
}

void RenderCore::Submit(VertexArray *pVertexArray, Shader *pShader)
{
	pVertexArray->Bind();
	pShader->Bind();
	m_pRenderAPI->DrawIndexed(pVertexArray->GetIndexBuffer()->GetCount());
}

} // namespace sl
