#include "FrameBuffer.h"

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"
#include "RenderCore/RenderCore.h"

namespace sl
{

FrameBuffer *FrameBuffer::Create(uint32_t width, uint32_t height)
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
			return new OpenGLFrameBuffer{ width, height };
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
