#pragma once

#include <cstdint>
#include <string>

namespace sl
{

class ModelImporter final
{
public:
	ModelImporter() = delete;
	ModelImporter(const ModelImporter &) = delete;
	ModelImporter &operator=(const ModelImporter &) = delete;
	ModelImporter(ModelImporter &&) = delete;
	ModelImporter &operator=(ModelImporter &&) = delete;
	~ModelImporter() = delete;

	static void Import(uint32_t entity, std::string_view path);
};

} // namespace sl
