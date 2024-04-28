#pragma once

#include "RenderCore/RenderContext.h"

namespace sl
{

class OpenGLContext : public RenderContext
{
public:
	OpenGLContext(void *pWindow);
	virtual ~OpenGLContext() override = default;

	virtual void Bind() override;
	virtual void SwapBuffers() override;

private:
	void *m_pWindow = nullptr;
};

} // namespace sl
