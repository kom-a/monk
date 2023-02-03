#pragma once

#include <inttypes.h>

#include "math/Math.h"

namespace monk
{
	class Random
	{
	public:
		Random() = delete;

	public:
		static void Seed(uint32_t seed);
		static int Next();
		static int Next(int min, int max);
		static float Normalized();
		static math::vec4 Color();
	private:
	};
}