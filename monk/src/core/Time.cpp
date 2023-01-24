#include "Time.h"

namespace monk
{
	Time::Time()
	{
		Reset();
	}

	void Time::Reset()
	{
		m_Start = std::chrono::high_resolution_clock::now();
		m_LastTime = std::chrono::high_resolution_clock::now();
	}

	float Time::Elapsed()
	{
		using namespace std::chrono;
		return duration_cast<nanoseconds>(high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
	}

	float Time::Delta()
	{
		using namespace std::chrono;
		auto currentTime = high_resolution_clock::now();
		float delta = duration_cast<nanoseconds>(currentTime - m_LastTime).count() * 0.001f * 0.001f * 0.001f;
		m_LastTime = currentTime;
		return delta;
	}

}


