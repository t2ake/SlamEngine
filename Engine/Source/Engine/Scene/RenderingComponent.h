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
	TextureResource *m_pTexture;
	Shader *m_pShader = nullptr;
};

} // namespace sl
