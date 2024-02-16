#include "Pattern.h"

#include <sstream>

namespace wm
{
	Pattern::Pattern(std::string_view pattern)
	{
		SplitPattern(pattern);
	}

	void Pattern::SplitPattern(std::string_view pattern)
	{
		m_Attributes.clear();

		enum class State
		{
			String,
			Percent
		};
		State state = State::String;

		std::stringstream ss;

		for (const char c : pattern)
		{
			switch (state)
			{
				case State::String:
				{
					if (c == '%')
						state = State::Percent;
					else
						ss << c;
				} break;
				case State::Percent:
				{
					switch (c)
					{
						case '%':
						{
							ss << '%';
							state = State::String;
						} break;
						case 'l':
						{
							Push(ss, Attribute::String);
							
							m_Attributes.emplace_back(Attribute::Level, std::string());
							state = State::String;
						} break;
						case 'n':
						{
							Push(ss, Attribute::String);

							m_Attributes.emplace_back(Attribute::Name, std::string());
							state = State::String;
						} break;
						case 'v':
						{
							Push(ss, Attribute::String);

							m_Attributes.emplace_back(Attribute::Value, std::string());
							state = State::String;
						} break;
						case 't':
						{
							Push(ss, Attribute::String);

							m_Attributes.emplace_back(Attribute::Time, std::string());
							state = State::String;
						} break;
						default:
						{
							ss << "%" << c;
						} break;
					}
				} break;
			}
		} 

		switch (state)
		{
			case State::String:
			{
				Push(ss, Attribute::String);
			} break;
			case State::Percent:
			{
				ss << "%";
				Push(ss, Attribute::String);
			} break;
		}
	}

	void Pattern::Push(std::stringstream& ss, Attribute attr)
	{
		if (ss.str().empty())
			return;

		m_Attributes.push_back(std::make_pair(attr, ss.str()));
		ss.str(std::string());
	}

}