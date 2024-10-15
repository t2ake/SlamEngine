#pragma once

#include "Core/Defines.h"

#include <map>
#include <string>
#include <vector>

namespace sl
{

struct VertexLayoutElement
{
	VertexLayoutElement(std::string_view name, AttribType type, uint32_t count, bool normalize = false);

	std::string m_name;
	uint32_t m_offset;
	uint32_t m_size;
	uint32_t m_count;
	AttribType m_type;
	bool m_normalize;
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
	uint32_t m_stride;
	std::vector<VertexLayoutElement> m_elements;
};

struct UniformBufferLayoutElement
{
	UniformBufferLayoutElement() = default;
	UniformBufferLayoutElement(AttribType type);

	uint32_t m_offset;
	uint32_t m_size;
};

struct UniformBufferLayoutElementInitor
{
	std::string_view m_name;
	AttribType m_type;
};

class UniformBufferLayout
{
public:
	UniformBufferLayout() = default;
	UniformBufferLayout(std::initializer_list<UniformBufferLayoutElementInitor> initors);

	void AddElement(std::string_view name, UniformBufferLayoutElement element);

	uint32_t GetOffste(std::string_view name) const;
	uint32_t GetSize(std::string_view name) const;
	uint32_t GetStride() const { return m_stride; }

	auto begin() { return m_elements.begin(); }
	auto end() { return m_elements.end(); }
	auto begin() const { return m_elements.cbegin(); }
	auto end() const { return m_elements.end(); }

private:
	// Will increase inside `AddElement`.
	uint32_t m_offset = 0;

	uint32_t m_stride = 0;
	std::map<std::string, UniformBufferLayoutElement> m_elements;
};

} // namespace sl
