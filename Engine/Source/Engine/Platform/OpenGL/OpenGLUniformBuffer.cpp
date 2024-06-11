#include "OpenGLUniformBuffer.h"

#include "Core/Log.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace sl
{

OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t bindingPoint, UniformBufferLayout layout) :
	m_layout(std::move(layout))
{
	glGenBuffers(1, &m_handle);
	glBindBuffer(GL_UNIFORM_BUFFER, m_handle);
	glBufferData(GL_UNIFORM_BUFFER, m_layout.GetStride(), nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_handle);
}

void OpenGLUniformBuffer::Bind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_handle);
}

void OpenGLUniformBuffer::Unbind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OpenGLUniformBuffer::Upload(std::string_view name, const glm::vec4 &value) const
{
	SL_ENGINE_ASSERT(m_layout.GetSize(name) == sizeof(value));
	glBufferSubData(GL_UNIFORM_BUFFER, m_layout.GetOffste(name), m_layout.GetSize(name), glm::value_ptr(value));
}

void OpenGLUniformBuffer::Upload(std::string_view name, const glm::mat4 &value) const
{
	SL_ENGINE_ASSERT(m_layout.GetSize(name) == sizeof(value));
	glBufferSubData(GL_UNIFORM_BUFFER, m_layout.GetOffste(name), m_layout.GetSize(name), glm::value_ptr(value));
}

} // namespace sl
