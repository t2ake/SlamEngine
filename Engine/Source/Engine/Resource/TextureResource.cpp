#include "TextureResource.h"

#include "Core/Log.h"
#include "Core/EnumOf.hpp"
#include "RenderCore/Texture.h"

#include <stb/stb_image.h>

namespace sl
{

namespace
{

static constexpr uint32_t TextureFormatChanles[nameof::enum_count<TextureFormat>()] =
{
	3,
	4,
};

static constexpr uint32_t TextureFormatBytes[nameof::enum_count<TextureFormat>()] =
{
	1,
	1,
};

}

TextureResource::TextureResource(std::string_view path, TextureFormat format, uint32_t flags) :
	m_filePath(path)
{
	// The first pixel should at the bottom left.
	stbi_set_flip_vertically_on_load(true);

	uint32_t requiredChanles = TextureFormatChanles[(size_t)format];
	uint32_t requiredBytes = TextureFormatBytes[(size_t)format];

	int width, height, channels;
	stbi_uc *pData = nullptr;
	switch(requiredBytes)
	{
		case 1:
		{
			pData = stbi_load(m_filePath.c_str(), &width, &height, &channels, requiredChanles);
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
