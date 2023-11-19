#include "Logger.h"

#include <iostream>
#include <sstream>

namespace wm
{
	Logger::Logger()
		: m_Pattern("%v")
	{
	}

	void Logger::SetPattern(const std::string& pattern)
	{
		m_Pattern.SplitPattern(pattern);
	}

	void Logger::SetName(const std::string& name)
	{
		m_Name = name;
	}

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

	void Logger::Warn(const std::string& fmt)
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

}


