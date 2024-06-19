#include "Layout.h"

#include "Core/EnumOf.hpp"
#include "Core/Log.h"

namespace sl
{

namespace
{

constexpr uint32_t AttribTypeSize[nameof::enum_count<AttribType>()] =
{
	1,  // AttribType::Int8
	1,  // AttribType::Uint8
	2,  // AttribType::Int16
	2,  // AttribType::Uint16
	4,  // AttribType::Int32
	4,  // AttribType::Uint32
	2,  // AttribType::Half
	4,  // AttribType::Float
	8,  // AttribType::Double
	16, // AttribType::vec4f
	64, // AttribType::mat4f
};

} // namespace

VertexLayoutElement::VertexLayoutElement(std::string_view name, AttribType type, uint32_t count, bool normalize) :
	m_name(name), m_offset(0) , m_size(AttribTypeSize[(size_t)type] * count), m_count(count), m_type(type), m_normalize(normalize)
{

}

VertexLayout::VertexLayout(std::initializer_list<VertexLayoutElement> elements) :
	m_stride(0), m_elements(std::move(elements))
{
	uint32_t offset = 0;
	for (auto &element : m_elements)
	{
		element.m_offset = offset;
		offset += element.m_size;
		m_stride += element.m_size;
	}
}

UniformBufferLayoutElement::UniformBufferLayoutElement(AttribType type) :
	m_offset(0), m_size(AttribTypeSize[(size_t)type])
{

}

UniformBufferLayout::UniformBufferLayout(std::initializer_list<UniformBufferLayoutElementInitor> initors)
{
	uint32_t offset = 0;
	for (auto &initor : initors)
	{
		SL_ASSERT(m_elements.find(initor.m_name.data()) == m_elements.end(), "Uniform buffer element already exists!");

		UniformBufferLayoutElement element;
		element.m_size = AttribTypeSize[(size_t)initor.m_type];
		element.m_offset = offset;
		offset += element.m_size;
		m_stride += element.m_size;

		m_elements[initor.m_name.data()] = std::move(element);
	}
}

void UniformBufferLayout::AddElement(std::string_view name, UniformBufferLayoutElement element)
{
	SL_ASSERT(m_elements.find(name.data()) == m_elements.end(), "Uniform buffer element already exists!");

	element.m_offset = m_offset;
	m_offset += element.m_size;
	m_stride += element.m_size;

	m_elements[name.data()] = std::move(element);
}

uint32_t UniformBufferLayout::GetOffste(std::string_view name) const
{
	SL_ASSERT(m_elements.find(name.data()) != m_elements.end(), "Uniform buffer element does not exist!");

	return m_elements.at(name.data()).m_offset;
}

uint32_t UniformBufferLayout::GetSize(std::string_view name) const
{
	SL_ASSERT(m_elements.find(name.data()) != m_elements.end(), "Uniform buffer element does not exist!");

	return m_elements.at(name.data()).m_size;
}

} // namespace sl
