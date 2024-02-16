#pragma once

#include "Pattern.h"
#include "Format.h"

#include <string>
#include <sstream>

namespace wm
{
	class Logger
	{
	public:
		Logger(std::string_view name, std::string_view pattern);
		~Logger() = default;

	public:
		enum class Level
		{
			Trace		= 0,
			Debug		= 1,
			Info		= 2,
			Warning		= 3,
			Error		= 4,
			Critical	= 5
		};

	public:
		void SetPattern(std::string_view pattern);
		void SetName(std::string_view name);
		void SetLevel(Level level);

		template <typename... Args>
		void Trace(std::string_view fmt, Args... args);

		template <typename... Args>
		void Debug(std::string_view fmt, Args... args);

		template <typename... Args>
		void Info(std::string_view fmt, Args... args);
		
		template <typename... Args>
		void Warning(std::string_view fmt, Args... args);

		template <typename... Args>
		void Error(std::string_view fmt, Args... args);

		template <typename... Args>
		void Critical(std::string_view fmt, Args... args);

	private:
		template <typename... Args>
		void Log(Level level, std::string_view fmt, Args... args);

		void PrintCurrentTime(std::ostream& stream) const;
		std::string LevelToString(Level level) const;

		std::string GetLevelColorCode(Level level) const;
		void SetColor(Level level, std::ostream& stream) const;
		void ResetColor(std::ostream& stream) const;

	private:
		Pattern m_Pattern = Pattern("%v");
		std::string m_Name = "Logger";
		Level m_Level = Level::Trace;
	};

	template <typename... Args>
	void Logger::Log(Level level, std::string_view fmt, Args... args)
	{
		Format format(fmt, args...);
		std::stringstream ss;

		SetColor(level, ss);
		
		for (const auto& [attr, value] : m_Pattern.m_Attributes)
		{
			switch (attr)
			{
				case Attribute::String:
				{
					ss << value;
				} break;
				case Attribute::Name:
				{
					ss << m_Name;
				} break;
				case Attribute::Level:
				{
					ss << LevelToString(level);
				} break;
				case Attribute::Value:
				{
					format.Print(ss);
				} break;
				case Attribute::Time:
				{
					PrintCurrentTime(ss);
				} break;
			}
		}

		ResetColor(ss);

		std::cout << ss.str() << std::endl;
	}

	template <typename... Args>
	void Logger::Trace(std::string_view fmt, Args... args)
	{
		if (m_Level > Level::Trace)
			return;

		Log(Level::Trace, fmt, args...);
	}

	template <typename... Args>
	void Logger::Debug(std::string_view fmt, Args... args)
	{
		if (m_Level > Level::Debug)
			return;

		Log(Level::Debug, fmt, args...);
	}

	template <typename... Args>
	void Logger::Info(std::string_view fmt, Args... args)
	{
		if (m_Level > Level::Info)
			return;

		Log(Level::Info, fmt, args...);
	}

	template <typename... Args>
	void Logger::Warning(std::string_view fmt, Args... args)
	{
		if (m_Level > Level::Warning)
			return;

		Log(Level::Warning, fmt, args...);
	}

	template <typename... Args>
	void Logger::Error(std::string_view fmt, Args... args)
	{
		if (m_Level > Level::Error)
			return;

		Log(Level::Error, fmt, args...);
	}

	template <typename... Args>
	void Logger::Critical(std::string_view fmt, Args... args)
	{
		Log(Level::Critical, fmt, args...);
	}
}