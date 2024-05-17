#include "Shader.h"

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "RenderCore/RenderCore.h"

namespace sl
{

Shader *Shader::Creat(std::string_view name, std::string_view vsSrc, std::string_view fsSrc)
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
			return new OpenGLShader(name, vsSrc, fsSrc);
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
