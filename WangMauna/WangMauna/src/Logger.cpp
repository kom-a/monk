#include "Logger.h"

#include <iostream>
#include <sstream>

#include <chrono>
#include <ctime>
#include <iomanip>

namespace wm
{
	Logger::Logger(std::string_view name, std::string_view pattern)
	{
		SetName(name);
		SetPattern(pattern);
	}

	void Logger::SetPattern(std::string_view pattern)
	{
		m_Pattern.SplitPattern(pattern);
	}

	void Logger::SetName(std::string_view name)
	{
		m_Name = name;
	}

	void Logger::SetLevel(Level level)
	{
		m_Level = level;
	}

	void Logger::PrintCurrentTime(std::ostream& stream) const
	{
		using namespace std::chrono;

		// get current time
		auto now = system_clock::now();

		// convert to std::time_t in order to convert to std::tm (broken time)
		auto timer = system_clock::to_time_t(now);

		// convert to broken time
		std::tm bt;
		::localtime_s(&bt, &timer);

		stream << std::put_time(&bt, "%H:%M:%S"); // HH:MM:SS
	}

	std::string Logger::LevelToString(enum class Level level) const
	{
		switch (level)
		{
			case Level::Trace:		return "TRACE";
			case Level::Debug:		return "DEBUG";
			case Level::Info:		return "INFO";
			case Level::Warning:	return "WARNING";
			case Level::Error:		return "ERROR";
			case Level::Critical:	return "CRITICAL";
		}

		return std::string();
	}

	std::string Logger::GetLevelColorCode(Level level) const
	{
		switch (level)
		{
			case Level::Trace:		return "\x1B[97m";
			case Level::Debug:		return "\x1B[94m";
			case Level::Info:		return "\x1B[92m";
			case Level::Warning:	return "\x1B[93m";
			case Level::Error:		return "\x1B[91m";
			case Level::Critical:	return "\x1B[97;101m";
		}
	}

	void Logger::SetColor(Level level, std::ostream& stream) const
	{
		stream << GetLevelColorCode(level);
	}

	void Logger::ResetColor(std::ostream& stream) const
	{
		stream << "\033[0m";
	}

#if 0
	void Logger::Trace(const std::string& fmt)
	{
		std::stringstream ss;

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
					ss << "DEBUG";
				} break;
				case Attribute::Value:
				{
					ss << fmt;
				} break;
				case Attribute::Time:
				{
					ss << "02:11:55";
				} break;
				case Attribute::Filename:
				{
					ss << "Main.cpp";
				} break;
				{
				case Attribute::Filepath:
					ss << "D:/dev/Monk/WangMuana/Sandbox/src/Main.cpp";
				} break;
				case Attribute::Fileline:
				{
					ss << "24";
				} break;
			}
		}

		std::cout << ss.str() << std::endl;
	}

	void Logger::Info(const std::string& fmt)
	{
		Trace(fmt);
	}

	void Logger::Debug(const std::string& fmt)
	{
		Trace(fmt);
	}

	void Logger::Warning(const std::string& fmt)
	{
		Trace(fmt);
	}

	void Logger::Error(const std::string& fmt)
	{
		Trace(fmt);
	}

	void Logger::Critical(const std::string& fmt)
	{
		Trace(fmt);
	}
#endif
}


