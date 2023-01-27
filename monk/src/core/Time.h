#pragma once

#include <chrono>

namespace monk
{
	class Time
	{
	public:
		Time();
		~Time() = default;

	public:
		void Reset();
		float Elapsed();
		float Delta();
	public:
		static long long CurrentTime();

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_LastTime;
	};
}