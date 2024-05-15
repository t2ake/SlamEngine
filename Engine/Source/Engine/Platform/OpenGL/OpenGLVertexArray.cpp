#include "OpenGLVertexArray.h"

#include "Core/EnumOf.hpp"
#include "Core/Log.h"

#include <glad/glad.h>

namespace sl
{

namespace
{

static constexpr GLenum AttribTypeToOpenGLType[nameof::enum_count<AttribType>()] =
{
	GL_BYTE,           // AttribType::Int8
	GL_UNSIGNED_BYTE,  // AttribType::Uint8
	GL_SHORT,          // AttribType::Int16
	GL_UNSIGNED_SHORT, // AttribType::Uint16
	GL_INT,            // AttribType::Int32
	GL_UNSIGNED_INT,   // AttribType::Uint32
	GL_HALF_FLOAT,     // AttribType::Half
	GL_FLOAT,          // AttribType::Float
	GL_DOUBLE,         // AttribType::Double
};

}

OpenGLVertexArray::OpenGLVertexArray()
{
	glCreateVertexArrays(1, &m_handle);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
	delete m_pVertexBuffer;
	delete m_pIndexBuffer;

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
	SL_ENGINE_ASSERT_INFO(pVertexBuffer->GetLayout().GetElementCount(), "VertexBuffer has no layout!");

	glBindVertexArray(m_handle);

	m_pVertexBuffer = pVertexBuffer;
	m_pVertexBuffer->Bind();

	uint32_t index = 0;
	const VertexLayout &layout = m_pVertexBuffer->GetLayout();
	for (const auto &element : layout)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(
			index,
			element.m_count,
			AttribTypeToOpenGLType[(size_t)element.m_type],
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

	m_pIndexBuffer = pIndexBuffer;
	m_pIndexBuffer->Bind();

	glBindVertexArray(0);
}

} // namespace sl
