#pragma once

#include "RenderCore/Layout.h"
#include "RenderCore/UniformBuffer.h"

namespace sl
{

class OpenGLUniformBuffer : public UniformBuffer
{
public:
	OpenGLUniformBuffer(uint32_t bindingPoint, UniformBufferLayout layout);

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void Upload(std::string_view name, const glm::vec4 &value) const override;
	virtual void Upload(std::string_view name, const glm::mat4 &value) const override;

private:
	UniformBufferLayout m_layout;

	uint32_t m_handle;
};

} // namespace sl
