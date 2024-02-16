#pragma once

#include <string>
#include <vector>

namespace wm
{
	enum class Attribute 
	{
		String,
		Level,
		Name,
		Value,
		Time
	};

	class Pattern
	{
	public:
		explicit Pattern(std::string_view pattern);
		~Pattern() = default;

	private:
		void SplitPattern(std::string_view pattern);
		void Push(std::stringstream& ss, Attribute attr);

	private:
		friend class Logger;
		std::vector<std::pair<Attribute, std::string>> m_Attributes;
	};
}
