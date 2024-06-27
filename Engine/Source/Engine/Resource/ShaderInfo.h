#pragma once

#include "Core/Defines.h"

#include <string>
#include <vector>

namespace sl
{

struct ShaderInfo
{
	ShaderType m_type;
	std::string m_name;
	std::string m_assetPath;
	std::string m_rowData;
};

} // namespace sl
