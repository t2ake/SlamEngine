#pragma once

#include "RenderCore/Texture.h"

namespace sl
{

class OpenGLTexture2D : public Texture2D
{
public:
	OpenGLTexture2D(std::string path, bool mipmap, uint32_t flags = 0);
	virtual ~OpenGLTexture2D() override;

	virtual uint32_t GetHeight() const override { return m_height; }
	virtual uint32_t GetWidth() const override { return m_width; }

	virtual void Bind(uint32_t slot) const override;

private:
	uint32_t m_handle = 0;

	uint32_t m_height = 0;
	uint32_t m_width = 0;
	std::string m_path;
};

} // namespace sl
