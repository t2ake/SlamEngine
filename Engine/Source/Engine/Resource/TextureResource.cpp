#include "TextureResource.h"

#include "Core/EnumOf.hpp"
#include "Core/Log.h"
#include "Core/Path.hpp"
#include "RenderCore/Texture.h"

#include <stb/stb_image.h>

namespace sl
{

namespace
{

static constexpr uint32_t TextureFormatChanles[nameof::enum_count<TextureFormat>()] =
{
	1, // TextureFormat::R8
	1, // TextureFormat::R16
	2, // TextureFormat::RG8
	2, // TextureFormat::RG16
	3, // TextureFormat::RGB8
	3, // TextureFormat::RGB16
	4, // TextureFormat::RGBA8
	4, // TextureFormat::RGBA16
	1, // TextureFormat::R32F
	2, // TextureFormat::RG32F
	3, // TextureFormat::RGB32F
	4, // TextureFormat::RGBA32F

	0,
	0,
	0,
	0,
	0,
	0,
};

static constexpr uint32_t TextureFormatBytes[nameof::enum_count<TextureFormat>()] =
{
	1, // TextureFormat::R8
	2, // TextureFormat::R16
	1, // TextureFormat::RG8
	2, // TextureFormat::RG16
	1, // TextureFormat::RGB8
	2, // TextureFormat::RGB16
	1, // TextureFormat::RGBA8
	2, // TextureFormat::RGBA16
	4, // TextureFormat::R32F
	4, // TextureFormat::RG32F
	4, // TextureFormat::RGB32F
	4, // TextureFormat::RGBA32F

	0,
	0,
	0,
	0,
	0,
	0,
};

}

TextureResource::TextureResource(std::string_view path, TextureFormat format, uint32_t flags) :
	m_filePath(path)
{
	if (!Path::Exists(m_filePath.c_str()))
	{
		SL_ENGINE_ERROR("\"{}\" does not exist!", m_filePath.c_str());
		return;
	}

	SL_ENGINE_TRACE("Loading {} texture \"{}\"", (stbi_is_hdr(m_filePath.c_str()) ? "HDR" : "LDR"), m_filePath.c_str());

	// The first pixel should at the bottom left.
	stbi_set_flip_vertically_on_load(true);

	uint32_t requiredChanles = TextureFormatChanles[(size_t)format];
	uint32_t requiredBytes = TextureFormatBytes[(size_t)format];

	int width, height, channels;
	void *pData = nullptr;
	switch(requiredBytes)
	{
		case 1:
		{
			pData = stbi_load(m_filePath.c_str(), &width, &height, &channels, requiredChanles);
			break;
		}
		case 2:
		{
			pData = stbi_load_16(m_filePath.c_str(), &width, &height, &channels, requiredChanles);
			break;
		}
		case 4:
		{
			pData = stbi_loadf(m_filePath.c_str(), &width, &height, &channels, requiredChanles);
			break;
		}
		default:
		{
			SL_ENGINE_ASSERT(false);
			break;
		}
	}

	if (!pData)
	{
		SL_ENGINE_ERROR("Failed to load image \"{}\"", m_filePath);
		return;
	}
	SL_ENGINE_ASSERT(width > 0 && height > 0 && channels > 0);

	m_width = (uint32_t)width;
	m_height = (uint32_t)height;
	size_t size = m_width * m_height * channels * requiredBytes;
	m_data.resize(size);
	memcpy(m_data.data(), pData, size);
	stbi_image_free(pData);

	m_pTexture = Texture2D::Create(m_width, m_height, true, format, flags, m_data.data());
}

TextureResource::~TextureResource()
{
	delete m_pTexture;
}

} //namespace sl
