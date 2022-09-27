#pragma once

#include <iostream>
#include <assert.h>
#include <vector>
#include <string>
#include <sstream>

namespace monk
{
	class Log
	{
	public:
		static Log& GetInstance();

	private:
		Log();
		~Log() = default;

	public:
		Log(const Log&) = delete;
		Log(const Log&&) = delete;
		void operator=(const Log&) = delete;
		void operator=(const Log&&) = delete;

	public:
		enum class Level
		{
			TRACE,
			INFO,
			WARN,
			ERR,
			CRITICAL
		};

	public:
		inline void SetLevel(Level level) { m_Level = level; }

		void Trace(const std::string& fmt);
		void Info(const std::string& fmt);
		void Warn(const std::string& fmt);
		void Error(const std::string& fmt);
		void Critical(const std::string& fmt);
		void Die(const std::string& fmt);

		template<typename... Args>
		void Trace(const std::string& fmt, Args... args);

		template<typename... Args>
		void Info(const std::string& fmt, Args... args);

		template<typename... Args>
		void Warn(const std::string& fmt, Args... args);

		template<typename... Args>
		void Error(const std::string& fmt, Args... args);

		template<typename... Args>
		void Critical(const std::string& fmt, Args... args);

		template<typename... Args>
		void Die(const std::string& fmt, Args... args);

	private:
		struct Token
		{
			enum Type
			{
				NONE,
				STRING,
				ARGUMENT
			} TokenType;

			std::string Value;
			int Index;

			Token()
				: TokenType(Type::NONE), Value(""), Index(-1)
			{ };
			~Token() { }
		};

		struct CollectArgumentData
		{
			int Value;
			std::string ReadString;
		};

		enum class Color
		{
			RESET = 0,

			FG_DEFAULT = 39,
			FG_WHITE = 37,
			FG_BLUE = 34,
			FG_CYAN = 36,
			FG_YELLOW = 33,
			FG_RED = 31,

			BG_DEFAULT = 49,
			BG_RED = 41
		};

	private:
		std::vector<Token*>* Parse(const std::string& fmt);
		bool CollectArgument(const char*& str, CollectArgumentData& outData);
		void Print(std::vector<Token*>* tokens);
		void Free(std::vector<Token*>* tokens);
		void SetColor(Color foreground, Color background);
		
		void LogMessage(const std::string& fmt);

		template<typename... Args>
		void LogMessage(const std::string& fmt, Args... args);

		template<typename T>
		void Prepare(const std::vector<Token*>* tokens, int index, T arg);

		template<typename T, typename... Args>
		void Prepare(const std::vector<Token*>* tokens, int index, T arg, Args... args);

	private:
		Level m_Level;
	};

	template<typename... Args>
	void Log::Trace(const std::string& fmt, Args... args)
	{
		if ((int)m_Level > (int)Level::TRACE)
			return;

		LogMessage(fmt, args...);
	}

	template<typename... Args>
	void Log::Info(const std::string& fmt, Args... args)
	{
		if ((int)m_Level > (int)Level::INFO)
			return;

		SetColor(Color::FG_CYAN, Color::BG_DEFAULT);
		LogMessage(fmt, args...);
		SetColor(Color::FG_DEFAULT, Color::BG_DEFAULT);
	}

	template<typename... Args>
	void Log::Warn(const std::string& fmt, Args... args)
	{
		if ((int)m_Level > (int)Level::WARN)
			return;

		SetColor(Color::FG_YELLOW, Color::BG_DEFAULT);
		LogMessage(fmt, args...);
		SetColor(Color::FG_DEFAULT, Color::BG_DEFAULT);
	}

	template<typename... Args>
	void Log::Error(const std::string& fmt, Args... args)
	{
		if ((int)m_Level > (int)Level::ERR)
			return;

		SetColor(Color::FG_RED, Color::BG_DEFAULT);
		LogMessage(fmt, args...);
		SetColor(Color::FG_DEFAULT, Color::BG_DEFAULT);
	}

	template<typename... Args>
	void Log::Critical(const std::string& fmt, Args... args)
	{
		if ((int)m_Level > (int)Level::CRITICAL)
			return;

		SetColor(Color::FG_WHITE, Color::BG_RED);
		LogMessage(fmt, args...);
		SetColor(Color::FG_DEFAULT, Color::BG_DEFAULT);
	}

	template<typename... Args>
	void Log::Die(const std::string& fmt, Args... args)
	{
		Critical(fmt, args...);
		exit(1);
	}

	template<typename... Args>
	void Log::LogMessage(const std::string& fmt, Args... args)
	{
		auto tokens = Parse(fmt);
		Prepare(tokens, 0, args...);
		Print(tokens);
		Free(tokens);
	}

	template<typename T>
	void Log::Prepare(const std::vector<Token*>* tokens, int index, T arg)
	{
		for (auto token : *tokens)
			if (token->Index == index && token->TokenType == Token::Type::ARGUMENT)
			{
				std::stringstream ss;
				ss << arg;

				token->TokenType = Token::Type::STRING;
				token->Value = ss.str();
			}
	}

	template<typename T, typename... Args>
	void Log::Prepare(const std::vector<Token*>* tokens, int index, T arg, Args... args)
	{
		for (auto token : *tokens)
			if (token->Index == index && token->TokenType == Token::Type::ARGUMENT)
			{
				std::stringstream ss;
				ss << arg;

				token->TokenType = Token::Type::STRING;
				token->Value = ss.str();
			}
		
		Prepare(tokens, index + 1, args...);
	}
}

#define LOG_TRACE(...) ::monk::Log::GetInstance().Trace(__VA_ARGS__)
#define LOG_INFO(...) ::monk::Log::GetInstance().Info(__VA_ARGS__)
#define LOG_WARN(...) ::monk::Log::GetInstance().Warn(__VA_ARGS__)
#define LOG_ERROR(...) ::monk::Log::GetInstance().Error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::monk::Log::GetInstance().Critical(__VA_ARGS__)
#define DIE(...) ::monk::Log::GetInstance().Die(__VA_ARGS__)
