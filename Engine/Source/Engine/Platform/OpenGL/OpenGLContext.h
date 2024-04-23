#pragma once

#include "RenderCore/RenderContext.h"

struct GLFWwindow;

namespace sl
{

class OpenGLContext : public RenderContext
{
public:
	OpenGLContext(GLFWwindow *pWindow);
	virtual ~OpenGLContext() override = default;

	virtual void Bind() override;
	virtual void SwapBuffers() override;

private:
	GLFWwindow *m_pWindow = nullptr;
};

} // namespace sl
