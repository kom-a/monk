#pragma once

#include <string>
#include <vector>

namespace monk::utils
{
	struct FileData
	{
		uint8_t* Data;
		size_t Size;

		void Free();
	};

	class FileManager
	{
	public:
		static std::string ReadFile(const char* filepath);
		static FileData ReadBytes(const std::string& filename);
	};
}
