#include "VertexArray.h"

#include "Log/Log.h"
#include "Platform/OpenGL/OpenGlVertexArray.h"
#include "RenderCore/RenderCore.h"

namespace sl
{

VertexArray *VertexArray::Create()
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
			return new OpenGLVertexArray;
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
