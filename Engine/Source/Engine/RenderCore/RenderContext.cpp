#include "RenderContext.h"

#include "Log/Log.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "RenderCore/RenderCore.h"

namespace sl
{

RenderContext *RenderContext::Create(GLFWwindow *pWindow)
{
	switch (RenderCore::GetInstance().GetBackend())
	{
		case GraphicsBackend::None:
		{
			SL_ENGINE_ASSERT(false);
			return nullptr;
		}
		case GraphicsBackend::OpenGL:
		{
			return new OpenGLContext{ pWindow };
			break;
		}
		default:
		{
			SL_ENGINE_ASSERT(false);
			return nullptr;
		}
	}
}

} // namespace sl
