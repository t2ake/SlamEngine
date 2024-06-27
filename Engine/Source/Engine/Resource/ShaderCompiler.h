#pragma once

#include "Resource/ShaderInfo.h"

namespace sl
{

// Basically its a combination of glslang, SPIRV-Tools and SPIRV-Cross
class ShaderCompiler final
{
public:
	ShaderCompiler() = delete;
	static std::string CompileShader(const ShaderInfo &info);
};

} // namespace sl
