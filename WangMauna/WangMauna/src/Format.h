#pragma once

#include <string>
#include <ostream>
#include <vector>
#include <variant>
#include <limits>

namespace wm
{
	template <typename... Args>
	class Format
	{
	public:
		explicit Format(std::string_view fmt, Args... args)
		{
			m_Format = BuildFormat(fmt, args...);
		}

		~Format() = default;

	public:
		void Print(std::ostream& stream)
		{
			stream << m_Format;
		}

	private:
		using Token = std::variant<std::string, uint32_t>;

		std::vector<Token> ParseFormat(std::string_view fmt)
		{
			std::vector<Token> tokens;
			std::stringstream ss;

			enum class State
			{
				String, 
				OpenBracket,
				CloseBracket,
				ArgumentIndex
			};

			State state = State::String;

			const uint32_t INVALID_INDEX = std::numeric_limits<uint32_t>::max();
			uint32_t argumentIndex = INVALID_INDEX;

			for (const auto& c : fmt)
			{
				switch (state)
				{
					case State::String:
					{
						if (c == '{')
						{
							state = State::OpenBracket;

							if(ss.tellp() != 0)
								tokens.push_back(ss.str());
							ss.str("");
						}
							
						ss << c;
					} break;
					case State::OpenBracket:
					{
						if (std::isdigit(c))
						{
							state = State::ArgumentIndex;
							argumentIndex = c - '0';
						}
						else
							state = State::String;

						ss << c;
					} break;
					case State::CloseBracket:
					{
						if (c == '{')
						{
							state = State::ArgumentIndex;
							argumentIndex = 0;
						}
						else 
							state = State::String;

						ss << c;
					} break;
					case State::ArgumentIndex:
					{
						if (std::isdigit(c))
						{
							argumentIndex *= 10;
							argumentIndex += c - '0';
						}
						else if (c == '}')
						{
							tokens.push_back(argumentIndex);
							argumentIndex = INVALID_INDEX;
							ss.str("");

							state = State::CloseBracket;
						}
						else
						{
							ss << '{' << std::to_string(argumentIndex);
							argumentIndex = INVALID_INDEX;
						}
					} break;
				}
			}

			if (ss.tellp() != 0)
				tokens.push_back(ss.str());

			return tokens;
		}

		std::string MergeArguments(const std::vector<Token> tokens, const std::vector<std::string> args)
		{
			std::stringstream ss;

			for (const auto& token : tokens)
			{
				if (std::holds_alternative<std::string>(token))
				{
					ss << std::get<std::string>(token);
				}
				else if (std::holds_alternative<uint32_t>(token))
				{
					uint32_t index = std::get<uint32_t>(token);
					if (index >= args.size())
						continue;

					ss << args[index];
				}
			}

			return ss.str();
		}
			 
		template <typename T>
		std::string ToString(const T& arg)
		{
			std::stringstream ss;
			ss << arg;
			return ss.str();
		}

		std::vector<std::string> ArgsToStrings()
		{
			return std::vector<std::string>();
		}

		template <typename T>
		std::vector<std::string> ArgsToStrings(const T& arg)
		{
			std::vector<std::string> result;
			result.push_back(ToString(arg));
			return result;
		}

		template <typename T, typename... Args>
		std::vector<std::string> ArgsToStrings(const T& arg, Args... args)
		{
			std::vector<std::string> result;
			result.push_back(ToString(arg));
			std::vector<std::string> rest = ArgsToStrings(args...);
			result.insert(result.end(), rest.begin(), rest.end());
			return result;
		}

		template <typename... Args>
		std::vector<std::string> ArgsToStrings(Args... args)
		{
			return ArgsToStrings(args...);
		}

		template <typename... Args>
		std::string BuildFormat(std::string_view fmt, Args... args)
		{
			const auto& tokens = ParseFormat(fmt);
			const auto& strArgs = ArgsToStrings(args...);
			
			std::string format = MergeArguments(tokens, strArgs);

			return format;
		}

	private:
		std::string m_Format;
	};
}
