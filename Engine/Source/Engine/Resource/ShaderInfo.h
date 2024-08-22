#pragma once

#include "Core/Defines.h"

#include <string>

namespace sl
{

struct ShaderInfo
{
	ShaderType m_type;
	bool m_debugMode;

	std::string m_name; // Shader file name without extension
	std::string m_assetPath;
	std::string m_binaryPath;

	std::string m_source;
};

} // namespace sl
