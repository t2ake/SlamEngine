#include "Shader.h"

#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "RenderCore/RenderCore.h"

namespace sl
{

Shader *Shader::Creat(uint32_t programHandle)
{
	switch (RenderCore::GetBackend())
	{
		case GraphicsBackend::None:
		{
			SL_ASSERT(false);
			return nullptr;
		}
		case GraphicsBackend::OpenGL:
		{
			return new OpenGLShader(programHandle);
			break;
		}
		default:
		{
			SL_ASSERT(false);
			return nullptr;
		}
	}
}

} // namespace sl
