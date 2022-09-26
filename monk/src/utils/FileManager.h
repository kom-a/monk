#pragma once

#include <string>
#include <vector>

namespace monk::utils
{
	class FileManager
	{
	public:
		static std::string ReadFile(const char* filepath);
		static std::vector<uint8_t> ReadBytes(const std::string& filename);
	};
}
