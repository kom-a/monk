#pragma once

#include <string>
#include <vector>

namespace wm
{
	enum class Attribute {
		String,
		Level,
		Name,
		Value,
		Time,
		// Date,
		// DateTime,
		Filename,
		Filepath,
		Fileline
	};

	class Pattern
	{
	public:

	public:
		Pattern(const std::string& pattern);
		~Pattern() = default;

		
	private:
		void SplitPattern(const std::string& pattern);
		void Push(std::stringstream& ss, Attribute attr);

	private:
		friend class Logger;
		std::vector<std::pair<Attribute, std::string>> m_Attributes;
	};
}
