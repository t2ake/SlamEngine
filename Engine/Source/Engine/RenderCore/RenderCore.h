#pragma once

#include "RenderCore/RenderAPI.h"
#include "RenderCore/Shader.h"

namespace sl
{

class RenderCore final
{
public:
	static RenderCore &GetInstance()
	{
		static RenderCore instance;
		return instance;
	}

public:
	RenderCore() = default;
	~RenderCore();

	void SetBackend(GraphicsBackend backend);
	GraphicsBackend GetBackend() { return m_backend; }

	void SetClearColor(float r, float g, float b, float a);
	void SetClearDepth(float depth);
	void SetClearStencil(int stencil);
	void Clear(uint8_t attachments);

	void Submit(VertexArray *pVertexArray, Shader *pShader);

private:
	GraphicsBackend m_backend = GraphicsBackend::None;
	RenderAPI *m_pRenderAPI = nullptr;
};

} // namespace sl