#pragma once

#include "RenderCore/RenderingContext.h"

struct GLFWwindow;

namespace sl
{

class OpenGLContext : public RenderingContext
{
public:
	OpenGLContext();

	virtual void Init(GLFWwindow *pWindow) override;
	virtual void SwapBuffers() override;
	virtual void Shutdown() override;

private:
	GLFWwindow *m_pWindow = nullptr;
};

} // namespace sl
