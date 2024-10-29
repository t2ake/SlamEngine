#include "OpenGLRenderAPI.h"

#include "Platform/OpenGL/OpenGLDefines.h"

namespace sl
{

BackendInfo OpenGLRenderAPI::GetBackendInfo()
{
	BackendInfo info;

	int maxTextureSize = 0;
	int maxFramebufferSize = 0;
	int maxFramebufferColorAttachmentCount = 0;
	int maxVertexUniformComponentCount = 0;
	int maxFragmentUniformComponentCount = 0;
	int maxUniformLocation = 0;
	int maxUniformBufferBinding = 0;

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxFramebufferSize);
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxFramebufferColorAttachmentCount);
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVertexUniformComponentCount);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFragmentUniformComponentCount);
	glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &maxUniformLocation);
	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxUniformBufferBinding);
	
	info.m_maxTextureSize = (uint32_t)maxTextureSize;
	info.m_maxFramebufferSize = (uint32_t)maxFramebufferSize;
	info.m_maxFramebufferColorAttachmentCount = (uint32_t)maxFramebufferColorAttachmentCount;
	info.m_maxVertexUniformComponentCount = (uint32_t)maxVertexUniformComponentCount;
	info.m_maxFragmentUniformComponentCount = (uint32_t)maxFragmentUniformComponentCount;
	info.m_maxUniformLocation = (uint32_t)maxUniformLocation;
	info.m_maxUniformBufferBinding = (uint32_t)maxUniformBufferBinding;

	return info;
}

void OpenGLRenderAPI::SetClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void OpenGLRenderAPI::SetClearDepth(float depth)
{
	glClearDepth(depth);
}

void OpenGLRenderAPI::SetClearStencil(int stencil)
{
	glClearStencil(stencil);
}

void OpenGLRenderAPI::ClearColor()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderAPI::ClearDepth()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderAPI::ClearStencil()
{
	glClear(GL_STENCIL_BUFFER_BIT);
}

void OpenGLRenderAPI::EnableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
}

void OpenGLRenderAPI::SetDepthBufferWriteable(bool writeable)
{
	glDepthMask(writeable);
}

void OpenGLRenderAPI::SetDepthFunc(CompareFunction func)
{
	glDepthFunc(GLCompareFunc[(size_t)func]);
}

void OpenGLRenderAPI::EnableBlend()
{
	glEnable(GL_BLEND);
}

void OpenGLRenderAPI::SetBlendFunc(BlendFunction sfactor, BlendFunction dfactor)
{
	glBlendFunc(GLBlendFunc[(size_t)sfactor], GLBlendFunc[(size_t)dfactor]);
}

void OpenGLRenderAPI::SetBlendColor(float r, float g, float b, float a)
{
	glBlendColor(r, g, b, a);
}

void OpenGLRenderAPI::EnableStencil()
{
	glEnable(GL_STENCIL_TEST);
}

void OpenGLRenderAPI::SetStencilMask(uint32_t mask)
{
	glStencilMask(mask);
}

void OpenGLRenderAPI::SetStencilFunc(CompareFunction func, int32_t ref, int32_t mask)
{
	glStencilFunc(GLCompareFunc[(size_t)func], ref, mask);
}

void OpenGLRenderAPI::SetStencilOp(StencilOperation fail, StencilOperation zfail, StencilOperation zpass)
{
	glStencilOp(GLStencilOp[(size_t)fail], GLStencilOp[(size_t)zfail], GLStencilOp[(size_t)zpass]);
}

void OpenGLRenderAPI::EnableCullFace()
{
	glEnable(GL_CULL_FACE);
}

void OpenGLRenderAPI::DisableCullFace()
{
	glDisable(GL_CULL_FACE);
}

void OpenGLRenderAPI::SetCullFace(CullingMode mode)
{
	glCullFace(GLCullingMode[(size_t)mode]);
}

void OpenGLRenderAPI::SetFrontFace(FrontFace face)
{
	glFrontFace(GLFrontFace[(size_t)face]);
}

void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	glViewport(x, y, width, height);
}

void OpenGLRenderAPI::DrawIndexed(uint32_t count)
{
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

} // namespace sl
