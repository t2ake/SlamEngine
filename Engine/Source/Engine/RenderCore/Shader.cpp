#include "Shader.h"

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "RenderCore/RenderCore.h"

namespace sl
{

Shader *Shader::Creat(std::string name, std::string vsSrc, std::string fsSrc)
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
			return new OpenGLShader(std::move(name), std::move(vsSrc), std::move(fsSrc));
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
