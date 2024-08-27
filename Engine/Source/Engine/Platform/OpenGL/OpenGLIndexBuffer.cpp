#include "OpenGLIndexBuffer.h"

#include "Utils/ProfilerGPU.h"

namespace sl
{

OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t *pIndices, size_t size)
{
	SL_PROFILE_GPU("glBufferData(IB)");

	m_count = uint32_t(size / sizeof(uint32_t));

	glCreateBuffers(1, &m_handle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, pIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
	glDeleteBuffers(1, &m_handle);
}

void OpenGLIndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
}

void OpenGLIndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace sl
