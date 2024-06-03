#include "VertexLayout.h"

#include "Core/EnumOf.hpp"

namespace sl
{

namespace
{

inline constexpr uint32_t AttribTypeSize[nameof::enum_count<AttribType>()] =
{
	1, // AttribType::Int8
	1, // AttribType::Uint8
	2, // AttribType::Int16
	2, // AttribType::Uint16
	4, // AttribType::Int32
	4, // AttribType::Uint32
	2, // AttribType::Half
	4, // AttribType::Float
	8, // AttribType::Double
};

}

VertexLayoutElement::VertexLayoutElement(std::string_view name, AttribType type, uint32_t count, bool normalize) :
	m_name(name), m_type(type), m_count(count), m_normalize(normalize)
{
	m_size = AttribTypeSize[(size_t)type] * m_count;
}

VertexLayout::VertexLayout(std::initializer_list<VertexLayoutElement> elements) :
	m_elements(std::move(elements))
{
	uint32_t offset = 0;
	m_stride = 0;
	for (auto &element : m_elements)
	{
		element.m_offset = offset;
		offset += element.m_size;
		m_stride += element.m_size;
	}
}

} // namespace sl
