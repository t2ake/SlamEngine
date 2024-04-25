#include "OpenGLRenderAPI.h"

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

void OpenGLRenderAPI::Clear(uint8_t attachments)
{
	uint32_t clearMask = 0;
	if (attachments & SL_CLEAR_COLOR)
	{
		clearMask |= GL_COLOR_BUFFER_BIT;
	}
	if (attachments & SL_CLEAR_DEPTH)
	{
		clearMask |= GL_DEPTH_BUFFER_BIT;
	}
	if (attachments & SL_CLEAR_STENCIL)
	{
		clearMask |= GL_STENCIL_BUFFER_BIT;
	}

	glClear(clearMask);
}

void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	glViewport(x, y, width, height);
}

void OpenGLRenderAPI::DefaultBlend()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRenderAPI::DrawIndexed(uint32_t count)
{
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

} // namespace sl
