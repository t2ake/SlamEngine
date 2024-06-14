#pragma once

#include <string>

namespace sl
{

class VertexArray;
class Shader;

struct RenderingComponent final
{
	void Reset()
	{

	}

	std::string m_textureResourceName;

	// TODO: Move these to ResourceManager
	VertexArray *m_pVertexArray = nullptr;
	Shader *m_pShader = nullptr;
	Shader *m_pIDShader = nullptr;
};

} // namespace sl
