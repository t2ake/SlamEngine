#include "TextureResource.h"

#include "Core/EnumOf.hpp"
#include "Core/Log.h"
#include "Core/Path.hpp"
#include "RenderCore/Texture.h"

#include <stb/stb_image.h>

namespace sl
{

TextureResource::TextureResource(std::string_view path, uint32_t flags) :
	m_filePath(path)
{
	if (!Path::Exists(m_filePath.c_str()))
	{
		SL_ENGINE_ERROR("File does not exist: \"{}\"", m_filePath.c_str());
		return;
	}
	SL_ENGINE_TRACE("Loading image: \"{}\"", m_filePath.c_str());

	// The first pixel should at the bottom left.
	stbi_set_flip_vertically_on_load(true);

	void *pData = nullptr;
	int width, height, channels;
	bool isHDR = stbi_is_hdr(m_filePath.c_str());
	if (isHDR)
	{
		pData = stbi_loadf(m_filePath.c_str(), &width, &height, &channels, 0);
	}
	else
	{
		pData = stbi_load(m_filePath.c_str(), &width, &height, &channels, 0);
	}

	if (!pData || width <= 0 || height <= 0 || channels <= 0)
	{
		SL_ENGINE_ERROR("Failed to load image!");
		return;
	}
	SL_ENGINE_TRACE("  Width: {}, Height: {}, Channels: {}, IsHDR: {}", width, height, channels, isHDR);

	m_width = (uint32_t)width;
	m_height = (uint32_t)height;
	size_t size = m_width * m_height * channels * (isHDR ? 4 : 1);
	m_data.resize(size);
	memcpy(m_data.data(), pData, size);
	stbi_image_free(pData);

	sl::TextureFormat format = sl::TextureFormat::RGB8;
	if (3 == channels && !isHDR)
	{
		format = sl::TextureFormat::RGB8;
	}
	else if (3 == channels && isHDR)
	{
		format = sl::TextureFormat::RGBA8;
	}
	else if (4 == channels && !isHDR)
	{
		format = sl::TextureFormat::RGB32F;
	}
	else if (4 == channels && isHDR)
	{
		format = sl::TextureFormat::RGBA32F;
	}
	else
	{
		SL_ENGINE_ERROR("Unknown image texture format!");
		m_data.clear();
		return;
	}

	m_pTexture = Texture2D::Create(m_width, m_height, true, format, flags, m_data.data());
}

TextureResource::~TextureResource()
{
	delete m_pTexture;
}

} //namespace sl
