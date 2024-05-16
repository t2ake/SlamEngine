#include "Texture.h"

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "RenderCore/RenderCore.h"

namespace sl
{

Texture2D *Texture2D::Create(std::string path, bool mipmap, uint32_t flags)
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
			return new OpenGLTexture2D{ std::move(path), mipmap, flags };
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
