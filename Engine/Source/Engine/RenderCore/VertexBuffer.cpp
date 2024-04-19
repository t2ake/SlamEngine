#include "VertexBuffer.h"

#include "Log/Log.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"
#include "RenderCore/RenderCore.h"

namespace sl
{

VertexBuffer *VertexBuffer::Create(const float *pVertices, size_t size)
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
			return new OpenGLVertexBuffer{ pVertices, size };
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
