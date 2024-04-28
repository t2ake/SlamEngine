#include "VertexLayout.h"

namespace sl
{

namespace
{

static constexpr uint32_t AttribTypeToSize[(size_t)AttribType::Count] =
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

VertexLayoutElement::VertexLayoutElement(std::string name, AttribType type, uint32_t count, bool normalize) :
	m_name(std::move(name)), m_type(type), m_count(count), m_normalize(normalize)
{
	m_size = AttribTypeToSize[(size_t)type] * m_count;
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
