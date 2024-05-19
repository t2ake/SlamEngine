#pragma once

#include <cstdint>
#include <vector>

namespace sl
{

class Texture2D;

class FrameBuffer
{
public:
	static FrameBuffer *Create(std::vector<Texture2D *> textures, bool destroy = false);

public:
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void Resize(uint32_t width, uint32_t height) = 0;
	
	virtual uint32_t GetWidth() const = 0;
	virtual uint32_t GetHeight() const = 0;

	virtual uint32_t GetColorAttachmentHandle(size_t i = 0) const = 0;
};

} // namespace sl
