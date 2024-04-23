#include "RenderAPI.h"

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLRenderAPI.h"

namespace sl
{

RenderAPI *RenderAPI::Create(GraphicsBackend backend)
{
	switch (backend)
	{
		case GraphicsBackend::None:
		{
			SL_ENGINE_ASSERT(false);
			return nullptr;
		}
		case GraphicsBackend::OpenGL:
		{
			return new OpenGLRenderAPI;
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
