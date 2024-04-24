#include "RenderAPI.h"

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLRenderAPI.h"
#include "RenderCore/RenderCore.h"

namespace sl
{

RenderAPI *RenderAPI::Create()
{
	switch (RenderCore::GetBackend())
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
