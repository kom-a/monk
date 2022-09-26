#include "utils/FileManager.h"

#include "core/Log.h"
#include "core/Assert.h"

#include <assert.h>
#include <fstream>


namespace monk::utils
{
	std::string FileManager::ReadFile(const char* filepath)
	{
		FILE* file;
		if (fopen_s(&file, filepath, "r"))
		{
			LOG_ERROR("Failed to open \"{0}\"", filepath);
			return "";
		}

		fseek(file, 0, SEEK_END);
		size_t length = ftell(file);
		char* data = new char[length + 1];
		memset(data, 0, length + 1);
		fseek(file, 0, SEEK_SET);
		fread(data, sizeof(char), length, file);
		fclose(file);

		std::string source(data);
		delete[] data;

		return source;
	}

	std::vector<uint8_t> FileManager::ReadBytes(const std::string& filename)
	{
		std::ifstream ifs(filename, std::ios::binary | std::ios::ate);

		MONK_ASSERT(ifs, "Failed to open file");

		auto end = ifs.tellg();
		ifs.seekg(0, std::ios::beg);

		auto size = std::size_t(end - ifs.tellg());

		if (size == 0)
			return {};

		std::vector<uint8_t> buffer(size);

		if (!ifs.read((char*)buffer.data(), buffer.size()))
			MONK_ASSERT(false, "Failed to read file");

		return buffer;
	}
}
