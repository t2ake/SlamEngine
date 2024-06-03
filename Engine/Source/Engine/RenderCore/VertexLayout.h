#pragma once

#include "Core/Defines.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace sl
{

struct VertexLayoutElement
{
	VertexLayoutElement(std::string_view name, AttribType type, uint32_t count, bool normalize = false);

	std::string m_name;
	AttribType m_type;
	uint32_t m_count;
	bool m_normalize;

	uint32_t m_size = 0;
	uint32_t m_offset = 0;
};

class VertexLayout final
{
public:
	VertexLayout() = default;
	VertexLayout(std::initializer_list<VertexLayoutElement> elements);

	uint32_t GetStride() const { return m_stride; }
	size_t GetElementCount() const { return m_elements.size(); }

	auto begin() { return m_elements.begin(); }
	auto end() { return m_elements.end(); }
	auto begin() const { return m_elements.cbegin(); }
	auto end() const { return m_elements.end(); }

private:
	std::vector<VertexLayoutElement> m_elements;
	uint32_t m_stride = 0;
};

} // namespace sl
