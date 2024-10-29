#pragma once

#include "RenderCore/BackendInfo.h"
#include "RenderCore/FrameBuffer.h"
#include "RenderCore/RenderAPI.h"
#include "RenderCore/Shader.h"
#include "RenderCore/Texture.h"
#include "RenderCore/UniformBuffer.h"
#include "RenderCore/VertexArray.h"

#include <memory>

namespace sl
{

class RenderCore final
{
public:
	RenderCore() = delete;
	RenderCore(const RenderCore &) = delete;
	RenderCore &operator=(const RenderCore &) = delete;
	RenderCore(RenderCore &&) = delete;
	RenderCore &operator=(RenderCore &&) = delete;
	~RenderCore() = delete;

	static void Init();

	static void SetBackend(GraphicsBackend backend) { m_backend = backend; }
	static GraphicsBackend GetBackend() { return m_backend; }

	static const BackendInfo &GetInfo() { return m_info; }
	
	static void SetMainFramebuffer(FrameBuffer *pBuffer) { m_pMainFrameBuffer.reset(pBuffer); }
	static FrameBuffer *GetMainFramebuffer() { return m_pMainFrameBuffer.get(); }

	static void SetEntityIDFramebuffer(FrameBuffer *pBuffer) { m_pEntityIDFrameBuffer.reset(pBuffer); }
	static FrameBuffer *GetEntityIDFramebuffer() { return m_pEntityIDFrameBuffer.get(); }

	static void SetUniformBuffer(std::string_view name, UniformBuffer *pUniformBuffer);
	static UniformBuffer *GetUniformBuffer(std::string_view name);

	static void SetDefaultState();

	static void ClearColor(float r, float g, float b, float a);
	static void ClearDepth(float depth);
	static void ClearStencil(int stencil);
	static void Culling(CullingMode mode);

	static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

	static void Submit(VertexArray *pVertexArray, Shader *pShader);

private:
	inline static GraphicsBackend m_backend = GraphicsBackend::None;
	inline static BackendInfo m_info = {};
	
	inline static std::unique_ptr<RenderAPI> m_pRenderAPI;
	inline static std::unique_ptr<FrameBuffer> m_pMainFrameBuffer;
	inline static std::unique_ptr<FrameBuffer> m_pEntityIDFrameBuffer;
	inline static std::map<std::string, std::unique_ptr<UniformBuffer>> m_UniformBuffers;
};

} // namespace sl
