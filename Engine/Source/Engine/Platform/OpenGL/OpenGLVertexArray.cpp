#include "OpenGLVertexArray.h"

#include "Core/EnumOf.hpp"
#include "Core/Log.h"
#include "Platform/OpenGL/OpenGLDefines.h"

#include <glad/glad.h>

namespace sl
{

OpenGLVertexArray::OpenGLVertexArray()
{
	glCreateVertexArrays(1, &m_handle);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
	glDeleteVertexArrays(1, &m_handle);
}

void OpenGLVertexArray::Bind() const
{
	glBindVertexArray(m_handle);
}

void OpenGLVertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void OpenGLVertexArray::SetVertexBuffer(VertexBuffer *pVertexBuffer)
{
	SL_ASSERT(pVertexBuffer->GetLayout().GetElementCount(), "VertexBuffer has no layout!");

	glBindVertexArray(m_handle);

	m_pVertexBuffer.reset(pVertexBuffer);
	m_pVertexBuffer->Bind();

	uint32_t index = 0;
	const VertexLayout &layout = m_pVertexBuffer->GetLayout();
	for (const auto &element : layout)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(
			index,
			element.m_count,
			GLAttribType[(size_t)element.m_type],
			element.m_normalize ? GL_TRUE : GL_FALSE,
			layout.GetStride(),
			(void *)(uint64_t)element.m_offset);
		index++;
	}

	glBindVertexArray(0);
}

void OpenGLVertexArray::SetIndexBuffer(IndexBuffer *pIndexBuffer)
{
	glBindVertexArray(m_handle);

	m_pIndexBuffer.reset(pIndexBuffer);
	m_pIndexBuffer->Bind();

	glBindVertexArray(0);
}

} // namespace sl
