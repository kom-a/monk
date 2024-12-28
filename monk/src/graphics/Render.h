#pragma once

#include <MOGL/MOGL.h>
#include <MML/MML.h>

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
			Front	= GL_FRONT,
			Back	= GL_BACK
		};

		enum class FrontFace
		{
			Clockwise			= GL_CW,
			CounterClockwise	= GL_CCW
		};

		static void SetClearColor(float r, float g, float b, float w);
		static void SetClearColor(const mml::vec4& clearColor);
		static void Clear();
		static void EnableDepthTest(bool enable = true);
		static void EnableBlending(bool enable = true);
		static void EnableCulling(bool enable = true, CullFace cullFace = CullFace::Back, FrontFace frontFace = FrontFace::Clockwise);

		static void Viewport(float x, float y, float width, float height);
		static void Viewport(float width, float height);
		static void Viewport(const mml::vec2& size);
		static void Viewport(const mml::vec4& viewport);
		

	private:
		static mml::vec4 s_ClearColor;
	};
}