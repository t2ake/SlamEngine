#pragma once

#include "Core/Log.h"

#include <fstream>
#include <string>
#include <vector>

namespace sl
{

class FileIO final
{
public:
	static std::vector<std::byte> LoadBin(std::string_view filePath)
	{
		std::vector<std::byte> fileData;

		std::ifstream in(filePath.data(), std::ios::in | std::ios::binary);
		if (!in)
		{
			SL_LOG_ERROR("Can not open file: \"{}\"", filePath.data());
			return fileData;
		}

		in.seekg(0, std::ios::end);
		fileData.resize(in.tellg());

		in.seekg(0, std::ios::beg);
		in.read((char *)fileData.data(), fileData.size());
		in.close();

		return fileData;
	}

	static std::string LoadStr(std::string_view filePath)
	{
		std::string fileData;

		std::ifstream in(filePath.data(), std::ios::in | std::ios::binary);
		if (!in)
		{
			SL_LOG_ERROR("Can not open file: \"{}\"", filePath.data());
			return fileData;
		}

		in.seekg(0, std::ios::end);
		fileData.resize(in.tellg());

		in.seekg(0, std::ios::beg);
		in.read(fileData.data(), fileData.size());
		in.close();

		return fileData;
	}
};

} // namespace sl
