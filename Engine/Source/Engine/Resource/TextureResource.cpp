#include "TextureResource.h"

#include "Core/Log.h"
#include "Core/Path.hpp"
#include "RenderCore/Texture.h"
#include "Resource/FileIO.hpp"
#include "Utils/ProfilerCPU.h"

#include <stb/stb_image.h>

namespace sl
{

TextureResource::TextureResource(std::string_view sourcePath, uint32_t flags) :
	sourcePath(sourcePath), m_flags(flags)
{

}

TextureResource::~TextureResource()
{
	OnDestroy();
}

void TextureResource::OnImport()
{
	SL_PROFILE;

	SL_LOG_TRACE("Loading image: \"{}\"", sourcePath.c_str());
	auto originalData = FileIO::ReadBinary(sourcePath);

	// The first pixel should at the bottom left.
	stbi_set_flip_vertically_on_load(true);

	void *pTextureData;
	int width, height, channel;
	bool getInfoSuccess = stbi_info_from_memory((stbi_uc *)originalData.data(), (int)originalData.size(), &width, &height, &channel);
	bool isHDR = stbi_is_hdr_from_memory((stbi_uc *)originalData.data(), (int)originalData.size());

	int desired_channels = 0;
	if (channel == 1)
	{
		desired_channels = 3;
		SL_LOG_TRACE("\tExpand 'Grey' to 'RGB'");
	}
	else if (channel == 2)
	{
		desired_channels = 4;
		SL_LOG_TRACE("\tExpand 'GreyAlpha' to 'RGBA'");
	}

	if (isHDR)
	{
		pTextureData = stbi_loadf_from_memory((stbi_uc *)originalData.data(), (int)originalData.size(),
			&width, &height, &channel, desired_channels);
	}
	else
	{
		pTextureData = stbi_load_from_memory((stbi_uc *)originalData.data(), (int)originalData.size(),
			&width, &height, &channel, desired_channels);
	}

#if !defined(SL_FINAL)
	if (!pTextureData || !getInfoSuccess || width <= 0 || height <= 0 || channel <= 0)
	{
		SL_LOG_ERROR("Invalid texture: \"{}\"\n\t{}", sourcePath.c_str(), stbi_failure_reason());
		m_state = ResourceState::Destroying;
		return;
	}
#endif

	m_width = (uint32_t)width;
	m_height = (uint32_t)height;
	m_channels = desired_channels ? (uint32_t)desired_channels : (uint32_t)channel;
	m_isHDR = isHDR;
	if (m_channels == 1 && !m_isHDR)
	{
		m_format = sl::TextureFormat::R8;
	}
	else if (m_channels == 1 && m_isHDR)
	{
		m_format = sl::TextureFormat::R32F;
	}
	else if (m_channels == 2 && !m_isHDR)
	{
		m_format = sl::TextureFormat::RG8;
	}
	else if (m_channels == 2 && m_isHDR)
	{
		m_format = sl::TextureFormat::RG32F;
	}
	else if (m_channels == 3 && !m_isHDR)
	{
		m_format = sl::TextureFormat::RGB8;
	}
	else if (m_channels == 3 && m_isHDR)
	{
		m_format = sl::TextureFormat::RGB32F;
	}
	else if (m_channels == 4 && !m_isHDR)
	{
		m_format = sl::TextureFormat::RGBA8;
	}
	else if (m_channels == 4 && m_isHDR)
	{
		m_format = sl::TextureFormat::RGBA32F;
	}
	else
	{
		SL_LOG_ERROR("Unknown texture format: \"{}\"", sourcePath.c_str());
		m_state = ResourceState::Destroying;
		return;
	}
	SL_LOG_TRACE("\tWidth: {}, Height: {}, Channels: {}, Format: {}, IsHDR: {}",
		m_width, m_height, m_channels, nameof::nameof_enum<sl::TextureFormat>(m_format), m_isHDR);

	m_rowData.resize(m_width * m_height * m_channels * (m_isHDR ? 4 : 1));
	memcpy(m_rowData.data(), pTextureData, m_rowData.size());
	stbi_image_free(pTextureData);

	m_state = ResourceState::Building;
}

void TextureResource::OnBuild()
{
	SL_PROFILE;

	m_state = ResourceState::Uploading;
}

void TextureResource::OnLoad()
{
	SL_PROFILE;

	m_state = ResourceState::Uploading;
}

void TextureResource::OnUpload()
{
	SL_PROFILE;

	m_pTexture.reset(Texture2D::Create(m_width, m_height, true, m_format, m_flags, m_rowData.data()));

	m_state = ResourceState::Ready;
}

void TextureResource::OnReady()
{
	if (m_destroyDelay <= 60 && m_destroyDelay++ == 60)
	{
		DestroyCPUData();
	}
}

void TextureResource::OnDestroy()
{
	SL_PROFILE;

	DestroyCPUData();
	m_pTexture.reset();
	
	m_state = ResourceState::Destroyed;
}

void TextureResource::DestroyCPUData()
{
	SL_PROFILE;

	m_rowData.clear();
	std::vector<std::byte>().swap(m_rowData);
}

} //namespace sl
