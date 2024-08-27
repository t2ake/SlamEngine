#include "OpenGLVertexBuffer.h"

#include "Utils/ProfilerGPU.h"

namespace sl
{

OpenGLVertexBuffer::OpenGLVertexBuffer(const float *pVertices, size_t size)
{
	SL_PROFILE_GPU("glBufferData(VB)");

	glCreateBuffers(1, &m_handle);
	glBindBuffer(GL_ARRAY_BUFFER, m_handle);
	glBufferData(GL_ARRAY_BUFFER, size, pVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
	glDeleteBuffers(1, &m_handle);
}

void OpenGLVertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_handle);
}

void OpenGLVertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace sl
