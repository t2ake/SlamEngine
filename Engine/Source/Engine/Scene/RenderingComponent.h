#pragma once

#include <optional>
#include <string>

namespace sl
{

class Shader;

struct RenderingComponent final
{
	void Reset()
	{

	}

	std::optional<std::string> m_optMeshResourceName;
	std::optional<std::string> m_optTextureResourceName;

	// TODO: Move these to ResourceManager
	Shader *m_pShader = nullptr;
	Shader *m_pIDShader = nullptr;
};

} // namespace sl
