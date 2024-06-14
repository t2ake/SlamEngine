#pragma once

#include "Core/Log.h"

#include <vector>
#include <fstream>

namespace sl
{

class ResourceLoader final
{
public:
	static std::vector<std::byte> LoadFile(const char *pFilePath)
	{
		std::vector<std::byte> fileData;

		std::ifstream in(pFilePath, std::ios::in | std::ios::binary);
		if (!in)
		{
			SL_ENGINE_ERROR("Can not open file: \"{}\"", pFilePath);
			return fileData;
		}

		in.seekg(0, std::ios::end);
		fileData.resize(in.tellg());

		in.seekg(0, std::ios::beg);
		in.read((char *)fileData.data(), fileData.size());
		in.close();

		return fileData;
	}
};

} // namespace sl
