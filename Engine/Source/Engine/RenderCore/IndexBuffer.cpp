#include "IndexBuffer.h"

#include "Log/Log.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"
#include "RenderCore/RenderCore.h"

namespace sl
{

IndexBuffer *IndexBuffer::Create(const uint32_t *pIndicies, size_t size)
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
			return new OpenGLIndexBuffer{ pIndicies, size };
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
