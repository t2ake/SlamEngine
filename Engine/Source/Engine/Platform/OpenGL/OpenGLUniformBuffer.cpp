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
	glBufferData(GL_UNIFORM_BUFFER, m_layout.GetSize(), nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_handle);
}

OpenGLUniformBuffer::~OpenGLUniformBuffer()
{
	glDeleteBuffers(1, &m_handle);
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
	auto optElement = m_layout.GetElement(name);
	if (optElement)
	{
		SL_ASSERT(optElement->m_size == sizeof(value));

		glBindBuffer(GL_UNIFORM_BUFFER, m_handle);
		glBufferSubData(GL_UNIFORM_BUFFER, optElement->m_offset, optElement->m_size, glm::value_ptr(value));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	else
	{
		SL_LOG_ERROR("Element {} does not exist!", name.data());
	}
}

void OpenGLUniformBuffer::Upload(std::string_view name, const glm::mat4 &value) const
{
	auto optElement = m_layout.GetElement(name);
	if (optElement)
	{
		SL_ASSERT(optElement->m_size == sizeof(value));

		glBindBuffer(GL_UNIFORM_BUFFER, m_handle);
		glBufferSubData(GL_UNIFORM_BUFFER, optElement->m_offset, optElement->m_size, glm::value_ptr(value));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	else
	{
		SL_LOG_ERROR("Element {} does not exist!", name.data());
	}
}

} // namespace sl
