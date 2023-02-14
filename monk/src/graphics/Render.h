#pragma once

#include "utils/OpenGL.h"
#include "Math/Math.h"

namespace monk
{
	class Render
	{
	private:
		Render() = default;
		~Render() = default;

	public:
		enum class CullFace
		{
			Front = GL_FRONT,
			Back = GL_BACK
		};

		enum class FrontFace
		{
			Clockwise = GL_CW,
			CounterClockwise = GL_CCW
		};

		static void SetClearColor(const math::vec4& clearColor);
		static void Clear();
		static void EnableDepthTest(bool enable = true);
		static void EnableBlending(bool enable = true);
		static void EnableCulling(bool enable = true, CullFace cullFace = CullFace::Back, FrontFace frontFace = FrontFace::Clockwise);

	private:
		static math::vec4 s_ClearColor;
	};
}