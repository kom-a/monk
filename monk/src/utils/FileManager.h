#pragma once

#include <string>

namespace monk::utils
{
	class FileManager
	{
	public:
		static std::string ReadFile(const char* filepath);
	};
}
