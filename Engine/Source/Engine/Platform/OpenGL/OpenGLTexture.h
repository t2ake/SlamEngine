#pragma once

#include "Core/Defines.h"
#include "RenderCore/Texture.h"

namespace sl
{

class OpenGLTexture2D : public Texture2D
{
public:
	OpenGLTexture2D(uint32_t width, uint32_t height, bool mipmap, TextureFormat format, uint32_t flags = 0, const void *pData = nullptr);
	virtual ~OpenGLTexture2D() override;

	virtual uint32_t GetWidth() const override { return m_width; }
	virtual uint32_t GetHeight() const override { return m_height; }

	virtual void Bind(uint32_t slot) const override;

private:
	uint32_t m_width = 0;
	uint32_t m_height = 0;

	bool m_hasMip;

	std::string m_path;

	uint32_t m_handle = 0;
};

} // namespace sl
