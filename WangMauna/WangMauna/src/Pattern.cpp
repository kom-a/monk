#include "Pattern.h"

#include <sstream>

namespace wm
{
	Pattern::Pattern(const std::string& pattern)
	{
		SplitPattern(pattern);
	}

	void Pattern::SplitPattern(const std::string& pattern)
	{
		m_Attributes.clear();

		enum class State
		{
			String,
			Percent,
			File,
		} state = State::String;

		std::stringstream ss;

		for (size_t i = 0; i < pattern.size(); i++)
		{
			char c = pattern[i];

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
							
							m_Attributes.push_back(std::make_pair(Attribute::Level, std::string()));
							state = State::String;
						} break;
						case 'n':
						{
							Push(ss, Attribute::String);

							m_Attributes.push_back(std::make_pair(Attribute::Name, std::string()));
							state = State::String;
						} break;
						case 'v':
						{
							Push(ss, Attribute::String);

							m_Attributes.push_back(std::make_pair(Attribute::Value, std::string()));
							state = State::String;
						} break;
						case 't':
						{
							Push(ss, Attribute::String);

							m_Attributes.push_back(std::make_pair(Attribute::Time, std::string()));
							state = State::String;
						} break;
						case 'f':
						{
							state = State::File;
						} break;
						default:
						{
							ss << "%" << c;
						} break;
					}
				} break;
				case State::File:
				{
					switch (c)
					{
						case 'n':
						{
							Push(ss, Attribute::String);

							m_Attributes.push_back(std::make_pair(Attribute::Filename, std::string()));
							state = State::String;
						} break;
						case 'p':
						{
							Push(ss, Attribute::String);

							m_Attributes.push_back(std::make_pair(Attribute::Filepath, std::string()));
							state = State::String;
						} break;
						case 'l':
						{
							Push(ss, Attribute::String);

							m_Attributes.push_back(std::make_pair(Attribute::Fileline, std::string()));
							state = State::String;
						} break;
						default:
						{
							ss << "%f" << c;
							state = State::String;
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
			case State::File:
			{
				ss << "%f";
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