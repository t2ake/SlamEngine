#include "TextureResource.h"

#include "Core/Log.h"
#include "Core/Path.hpp"
#include "RenderCore/Texture.h"
#include "Resource/FileIO.hpp"

#include <stb/stb_image.h>

namespace sl
{

TextureResource::TextureResource(std::string_view path, uint32_t flags) :
	m_assetPath(path), m_flags(flags)
{
	SetStatus(ResourceStatus::Importing);
}

TextureResource::~TextureResource()
{
	OnDestroy();
}

void TextureResource::OnImport()
{
	SL_LOG_TRACE("Loading image: \"{}\"", m_assetPath.c_str());
	const auto originalData = FileIO::ReadBinary(m_assetPath);

	// The first pixel should at the bottom left.
	stbi_set_flip_vertically_on_load(true);

	void *pTextureData;
	int width, height, channels;
	bool isHDR = stbi_is_hdr_from_memory((stbi_uc *)originalData.data(), (int)originalData.size());
	if (isHDR)
	{
		pTextureData = stbi_loadf_from_memory((stbi_uc *)originalData.data(), (int)originalData.size(), &width, &height, &channels, 0);
	}
	else
	{
		pTextureData = stbi_load_from_memory((stbi_uc *)originalData.data(), (int)originalData.size(), &width, &height, &channels, 0);
	}

	if (!pTextureData || width <= 0 || height <= 0 || channels <= 0)
	{
		SL_LOG_ERROR("Invalid texture: \"{}\"", m_assetPath.c_str());
		SetStatus(ResourceStatus::Destroying);
		return;
	}

	m_width = (uint32_t)width;
	m_height = (uint32_t)height;
	m_channels = (uint32_t)channels;
	m_isHDR = isHDR;
	SL_LOG_TRACE("  Width: {}, Height: {}, Channels: {}, IsHDR: {}", m_width, m_height, m_channels, m_isHDR);

	m_rowData.resize(m_width * m_height * m_channels * (m_isHDR ? 4 : 1));
	memcpy(m_rowData.data(), pTextureData, m_rowData.size());
	stbi_image_free(pTextureData);

	SetStatus(ResourceStatus::Building);
}

void TextureResource::OnBuild()
{
	SetStatus(ResourceStatus::Uploading);
}

void TextureResource::OnLoad()
{
	SetStatus(ResourceStatus::Uploading);
}

void TextureResource::OnUpload()
{
	sl::TextureFormat format = sl::TextureFormat::RGB8;
	if (m_channels == 3 && !m_isHDR)
	{
		format = sl::TextureFormat::RGB8;
	}
	else if (m_channels == 3 && m_isHDR)
	{
		format = sl::TextureFormat::RGB32F;
	}
	else if (m_channels == 4 && !m_isHDR)
	{
		format = sl::TextureFormat::RGBA8;
	}
	else if (m_channels == 4 && m_isHDR)
	{
		format = sl::TextureFormat::RGBA32F;
	}
	else
	{
		SL_LOG_ERROR("Unknown image texture format!");
		SetStatus(ResourceStatus::Destroying);
		return;
	}

	m_pTexture.reset(Texture2D::Create(m_width, m_height, true, format, m_flags, m_rowData.data()));

	SetStatus(ResourceStatus::Ready);
}

void TextureResource::OnReady()
{
	static uint8_t frameCount = 0;
	if (frameCount <= 60 && frameCount++ == 60)
	{
		DestroyCPUData();
	}
}

void TextureResource::OnDestroy()
{
	DestroyCPUData();
	m_pTexture.reset();
	
	SetStatus(ResourceStatus::Destroyed);
}

void TextureResource::DestroyCPUData()
{
	m_rowData.clear();
	std::vector<std::byte>().swap(m_rowData);
}

} //namespace sl
