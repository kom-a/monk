#pragma once

#include "Math/Math.h"

namespace monk
{
	class Render
	{
	private:
		Render() = default;
		~Render() = default;

	public:
		static void SetClearColor(const math::vec4& clearColor);
		static void Clear();
		static void EnableBlending(bool enable = true);

	private:
		static math::vec4 s_ClearColor;
	};
}