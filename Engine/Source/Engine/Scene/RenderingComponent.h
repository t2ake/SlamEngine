#pragma once

#include "Core/Log.h"

#include <vector>

namespace sl
{

class VertexArray;
class Texture2D;
class Shader;

struct RenderingComponent final
{
	void Reset()
	{

	}

	void SetTexture(sl::Texture2D *pTexture, size_t index)
	{
		SL_ENGINE_ASSERT_INFO(32 > index, "Only support 32 textures for now.");
		m_pTextures[index] = pTexture;
	}

	Texture2D *GetTexture(size_t index) const
	{
		SL_ENGINE_ASSERT_INFO(32 > index, "Only support 32 textures for now.");
		return m_pTextures.at(index);
	}
	
	VertexArray *m_pVertexArray = nullptr;
	std::vector<Texture2D *> m_pTextures{ 32, nullptr };
	Shader *m_pShader = nullptr;
};

} // namespace sl
