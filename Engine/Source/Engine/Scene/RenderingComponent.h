#pragma once

namespace sl
{

class VertexArray;
class TextureResource;
class Shader;

struct RenderingComponent final
{
	void Reset()
	{

	}

	VertexArray *m_pVertexArray = nullptr;
	TextureResource *m_pTextureResource = nullptr;
	Shader *m_pShader = nullptr;
	Shader *m_pIDShader = nullptr;
};

} // namespace sl
