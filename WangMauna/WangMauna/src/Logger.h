#pragma once

#include "Pattern.h"

#include <string>

namespace wm
{
	class Logger
	{
	public:
		Logger();
		~Logger() = default;

	public:
		void SetPattern(const std::string& pattern);
		void SetName(const std::string& name);

		void Trace(const std::string& fmt);
		void Info(const std::string& fmt);
		void Debug(const std::string& fmt);
		void Warn(const std::string& fmt);
		void Error(const std::string& fmt);
		void Critical(const std::string& fmt);

	private:
		Pattern m_Pattern;

		std::string m_Name = "Logger";
	};
}