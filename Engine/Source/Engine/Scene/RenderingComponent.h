#pragma once

#include <optional>
#include <string>

namespace sl
{

class Shader;

struct RenderingComponent final
{
	std::optional<std::string> m_optBaseShaderResourceName;
	std::optional<std::string> m_optIDShaderResourceName;
	std::optional<std::string> m_optTextureResourceName;
	std::optional<std::string> m_optMeshResourceName;
};

} // namespace sl
