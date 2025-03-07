#pragma once

#include <inttypes.h>
#include <random>

#include <MML/MML.h>

namespace monk
{
	class Random
	{
	public:
		Random() = delete;

	public:
		static int RandomInt(int min, int max);
		static float Normalized();

	private:
		static std::random_device m_RandomDevice;
	};
}