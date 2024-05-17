#include "OpenGLRenderAPI.h"

#include "Platform/OpenGL/OpenGLDefines.h"

#include <glad/glad.h>

namespace sl
{

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

void OpenGLRenderAPI::DepthBufferWriteable(bool writeable)
{
	glDepthMask(writeable);
}

void OpenGLRenderAPI::DepthFunc(CompareFunction func)
{
	glDepthFunc(GLCompareFunc[(size_t)func]);
}

void OpenGLRenderAPI::EnableBlend()
{
	glEnable(GL_BLEND);
}

void OpenGLRenderAPI::BlendFunc(BlendFunction sfactor, BlendFunction dfactor)
{
	glBlendFunc(GLBlendFunc[(size_t)sfactor], GLBlendFunc[(size_t)dfactor]);
}

void OpenGLRenderAPI::BlendColor(float r, float g, float b, float a)
{
	glBlendColor(r, g, b, a);
}

void OpenGLRenderAPI::EnableStencil()
{
	glEnable(GL_STENCIL_TEST);
}

void OpenGLRenderAPI::StencilMask(uint32_t mask)
{
	glStencilMask(mask);
}

void OpenGLRenderAPI::StencilFunc(CompareFunction func, int32_t ref, int32_t mask)
{
	glStencilFunc(GLCompareFunc[(size_t)func], ref, mask);
}

void OpenGLRenderAPI::StencilOp(StencilOperation fail, StencilOperation zfail, StencilOperation zpass)
{
	glStencilOp(GLStencilOp[(size_t)fail], GLStencilOp[(size_t)zfail], GLStencilOp[(size_t)zpass]);
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
