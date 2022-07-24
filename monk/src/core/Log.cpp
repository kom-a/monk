#include "core/Log.h"

namespace monk
{
	Log& Log::GetInstance()
	{
		static Log instance;

		return instance;
	}

	Log::Log()
		: m_Level(Level::TRACE)
	{

	}

	void Log::Trace(const std::string& fmt)
	{
		if ((int)m_Level > (int)Level::TRACE)
			return;

		LogMessage(fmt);
	}

	void Log::Info(const std::string& fmt)
	{
		if ((int)m_Level > (int)Level::INFO)
			return;

		SetColor(Color::FG_CYAN, Color::BG_DEFAULT);
		LogMessage(fmt);
		SetColor(Color::FG_DEFAULT, Color::BG_DEFAULT);
	}

	void Log::Warn(const std::string& fmt)
	{
		if ((int)m_Level > (int)Level::WARN)
			return;

		SetColor(Color::FG_YELLOW, Color::BG_DEFAULT);
		LogMessage(fmt);
		SetColor(Color::FG_DEFAULT, Color::BG_DEFAULT);
	}

	void Log::Error(const std::string& fmt)
	{
		if ((int)m_Level > (int)Level::ERR)
			return;

		SetColor(Color::FG_RED, Color::BG_DEFAULT);
		LogMessage(fmt);
		SetColor(Color::FG_DEFAULT, Color::BG_DEFAULT);
	}

	void Log::Critical(const std::string& fmt)
	{
		if ((int)m_Level > (int)Level::CRITICAL)
			return;

		SetColor(Color::FG_WHITE, Color::BG_RED);
		LogMessage(fmt);
		SetColor(Color::FG_DEFAULT, Color::BG_DEFAULT);
	}

	void Log::Die(const std::string& fmt)
	{
		Critical(fmt);
		exit(1);
	}

	std::vector<Log::Token*>* Log::Parse(const std::string& fmt)
	{	
		std::vector<Token*>* tokens = new std::vector<Token*>();

		const char* str = fmt.c_str();

		std::stringstream ss;

		while (*str != '\0')
		{
			if (*str != '{')
				ss << *str;
			else
			{
				CollectArgumentData collectData;

				if (CollectArgument(str, collectData))
				{
					Token* token = new Token();
					token->TokenType = Token::Type::STRING;
					token->Value = ss.str();
					ss.str(std::string());
					tokens->push_back(token);

					token = new Token();
					token->TokenType = Token::Type::ARGUMENT;
					token->Index = collectData.Value;
					tokens->push_back(token);
				}
				else
				{
					ss << collectData.ReadString;
				}
			}

			if(*str != '\0')
				str++;
		}
		
		Token* token = new Token();
		token->TokenType = Token::Type::STRING;
		token->Value = ss.str();
		tokens->push_back(token);

		return tokens;
	}

	bool Log::CollectArgument(const char*& str, CollectArgumentData& outData)
	{
		bool success = false;
		int value = 0;
		int digits = 0;
		outData.ReadString = *str++;
		
		while (*str != '\0')
		{
			outData.ReadString.append(1, *str);

			if (*str == '}')
			{
				success = digits > 0;
				break;
			}

			if (!isdigit(*str))
			{
				success = false;
				break;
			}

			value *= 10;
			value += *str - '0';
			digits++;
			str++;
		}

		outData.Value = success ? value : -1;
		return success;
	}

	void Log::Print(std::vector<Token*>* tokens)
	{
		for (auto token : *tokens)
		{
			std::cout << token->Value;
		}
		std::cout << std::endl;
	}

	void Log::Free(std::vector<Token*>* tokens)
	{
		auto vec = *tokens;
		for (size_t i = 0; i < vec.size(); i++)
			delete vec[i];

		delete tokens;
	}

	void Log::SetColor(Color foreground, Color background)
	{
		std::stringstream ss;
		ss << "\033[" << (int)foreground << ";" << (int)background << "m";

		std::cout << ss.str();
	}

	void Log::LogMessage(const std::string& fmt)
	{
		auto tokens = Parse(fmt);
		Print(tokens);
		Free(tokens);
	}

}