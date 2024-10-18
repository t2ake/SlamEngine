#pragma once

#include <optional>
#include <string>

namespace sl
{

class Shader;

struct RenderingComponent final
{
	std::optional<std::string> m_optMeshResourceName;
	std::optional<std::string> m_optMaterialResourceName;
	std::optional<std::string> m_optBaseShaderResourceName = "BaseShader";
	std::optional<std::string> m_optIDShaderResourceName = "IDShader";
};

} // namespace sl
