#pragma once

#include "RenderCore/RenderContext.h"

namespace sl
{

class OpenGLContext : public RenderContext
{
public:
	OpenGLContext(void *pWindow);
	virtual ~OpenGLContext() override;

	virtual void *GetContext() override { return m_pContext; }

	virtual void MakeCurrent() override;
	virtual void SwapBuffers() override;

private:
	void *m_pWindow = nullptr;
	void *m_pContext = nullptr;
};

} // namespace sl
