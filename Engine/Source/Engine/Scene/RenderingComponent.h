#pragma once

#include <optional>
#include <string>

namespace sl
{

class Shader;

struct RenderingComponent final
{
	std::optional<std::string> m_optBaseShaderResourceName = "BaseShader";
	std::optional<std::string> m_optIDShaderResourceName = "IDShader";
	std::optional<std::string> m_optTextureResourceName;
	std::optional<std::string> m_optMeshResourceName;
};

} // namespace sl
