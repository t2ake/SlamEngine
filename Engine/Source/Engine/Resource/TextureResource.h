#pragma once

#include "Core/Defines.h"

#include <string>
#include <vector>

namespace sl
{

class Texture2D;

class TextureResource
{
public:
	TextureResource() = default;
	TextureResource(std::string_view path, uint32_t flags = 0);
	~TextureResource();

	Texture2D *GetTexture() const { return m_pTexture; }

private:
	std::string m_filePath;

	uint32_t m_width = 0;
	uint32_t m_height = 0;
	// TODO: Compression Format

	std::vector<std::byte> m_data;

	Texture2D *m_pTexture;
};

} // namespace sl
