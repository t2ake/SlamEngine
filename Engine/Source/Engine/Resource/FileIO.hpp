#pragma once

#include "Core/Log.h"
#include "Utils/ProfilerCPU.h"

#include <nameof/nameof.hpp>

#include <fstream>
#include <string>
#include <vector>

namespace sl
{

class FileIO final
{
public:
	FileIO() = delete;
	FileIO(const FileIO &) = delete;
	FileIO &operator=(const FileIO &) = delete;
	FileIO(FileIO &&) = delete;
	FileIO &operator=(FileIO &&) = delete;
	~FileIO() = delete;

	template<class T = std::byte>
	static std::vector<T> ReadBinary(std::string_view filePath)
	{
		SL_PROFILE;

		std::vector<T> fileData;

		std::ifstream in(filePath.data(), std::ios::in | std::ios::binary);
		if (!in)
		{
			SL_LOG_ERROR("Failed to open file: \"{}\"", filePath.data());
			return fileData;
		}

		in.seekg(0, std::ios::end);
		size_t fileSize = in.tellg();
		if (fileSize % sizeof(T) != 0)
		{
			SL_LOG_ERROR("File size {} is not divisible by sizeof({})!", fileSize, nameof::nameof_type<T>());
			return fileData;
		}

		fileData.resize(fileSize / sizeof(T));
		in.seekg(0, std::ios::beg);
		in.read(reinterpret_cast<char *>(fileData.data()), fileSize);
		in.close();

		return fileData;
	}

	static std::string ReadString(std::string_view filePath)
	{
		SL_PROFILE;

		std::string fileData;

		std::ifstream in(filePath.data(), std::ios::in | std::ios::binary);
		if (!in)
		{
			SL_LOG_ERROR("Failed to open file: \"{}\"", filePath.data());
			return fileData;
		}

		in.seekg(0, std::ios::end);
		fileData.resize(in.tellg());

		in.seekg(0, std::ios::beg);
		in.read(fileData.data(), fileData.size());
		in.close();

		return fileData;
	}

	static void WriteBinary(std::string_view path, const char* pData, size_t size)
	{
		SL_PROFILE;

		if (!pData || size <= 0)
		{
			SL_LOG_ERROR("Failed to write file: \"{}\"", path.data());
			return;
		}

		std::ofstream out(path.data(), std::ios::out | std::ios::binary | std::ios::trunc);
		if (!out)
		{
			SL_LOG_ERROR("Failed to open file: \"{}\"", path.data());
			return;
		}

		out.write(pData, size);
		out.close();
	}
};

} // namespace sl
