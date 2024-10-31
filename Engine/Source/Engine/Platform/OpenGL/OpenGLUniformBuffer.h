#pragma once

#include "RenderCore/Layout.h"
#include "RenderCore/UniformBuffer.h"

namespace sl
{

class OpenGLUniformBuffer : public UniformBuffer
{
public:
	OpenGLUniformBuffer(uint32_t bindingPoint, UniformBufferLayout layout);
	virtual ~OpenGLUniformBuffer() override;

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void Upload(std::string_view name, const void *pData, uint32_t size = 0) const override;

private:
	UniformBufferLayout m_layout;

	uint32_t m_handle;
};

} // namespace sl
