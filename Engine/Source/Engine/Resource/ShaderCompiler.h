#pragma once

#include "Resource/ShaderInfo.h"

#include <vector>

namespace sl
{

// Basically, its just a combination of glslang, SPIRV-Tools and SPIRV-Cross.
class ShaderCompiler final
{
public:
	ShaderCompiler() = delete;
	
	static std::vector<uint32_t> SourceToSpirv(const ShaderInfo &info);
	static std::string SpirvToSource(std::vector<uint32_t> spirv);
	static void ReflectSpirv(const std::vector<uint32_t> &spirv);
};

} // namespace sl
