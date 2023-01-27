#include "Random.h"

#include <cstdlib>

#include "Time.h"
#include "Assert.h"

namespace monk
{
	int Random::Next()
	{
		return std::rand();
	}

	int Random::Next(int min, int max)
	{
		MONK_ASSERT(min < max, "Minimal value can not be greater than maximal");

		return min + std::rand() % (max - min);
	}

	float Random::Normalized()
	{
		return (float)std::rand() / RAND_MAX;
	}

	math::vec4 Random::Color()
	{
		return math::vec4(Normalized(), Normalized(), Normalized(), 1.0f);
	}
}