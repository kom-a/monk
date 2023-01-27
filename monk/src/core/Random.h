#pragma once

#include "math/Math.h"

namespace monk
{
	class Random
	{
	public:
		Random() = delete;

	public:
		static int Next();
		static int Next(int min, int max);
		static float Normalized();
		static math::vec4 Color();
	private:
	};
}