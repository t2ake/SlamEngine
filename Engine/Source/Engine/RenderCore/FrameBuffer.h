#pragma once

#include <cstdint>

namespace sl
{

class FrameBuffer
{
public:
	static FrameBuffer *Create(uint32_t width, uint32_t height);

public:
	virtual uint32_t GetWidth() const = 0;
	virtual uint32_t GetHeight() const = 0;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void Resize(uint32_t width, uint32_t height) = 0;

	virtual uint32_t GetColorAttachmentHandle() const = 0;
};

} // namespace sl
