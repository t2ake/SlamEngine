#pragma once

#include "Core/Log.h"
#include "Utils/ProfilerCPU.h"

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
		const size_t size = in.tellg();
		fileData.resize(size / sizeof(T));
		SL_ASSERT(size % sizeof(T) == 0, "Please ensure that the file size is an integer multiple of sizeof(T) by yourself.");

		in.seekg(0, std::ios::beg);
		in.read(reinterpret_cast<char *>(fileData.data()), size);
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

	static void WriteBinary(std::string_view path, char* pData, size_t size)
	{
		SL_PROFILE;
		SL_ASSERT(pData && size > 0);

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
