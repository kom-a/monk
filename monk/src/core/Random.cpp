#include "Random.h"

#include "Time.h"
#include "Assert.h"

namespace monk
{
	std::random_device Random::m_RandomDevice = std::random_device();

	int Random::RandomInt(int min, int max)
	{
		MONK_ASSERT(min < max, "Minimal value can not be greater than maximal");

		std::default_random_engine randomEngine(m_RandomDevice());
		std::uniform_int_distribution uniform_dist(min, max);

		return uniform_dist(randomEngine);
	}

	float Random::Normalized()
	{
		return (float)RandomInt(0, RAND_MAX)/ RAND_MAX;
	}
}