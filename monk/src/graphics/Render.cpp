#include "Render.h"

namespace monk
{
	mml::vec4 Render::s_ClearColor = mml::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	void Render::SetClearColor(float r, float g, float b, float w)
	{
		s_ClearColor = mml::vec4(r, g, b, w);
	}

	void Render::SetClearColor(const mml::vec4& clearColor)
	{
		s_ClearColor = clearColor;
	}

	void Render::Clear()
	{
		glClearColor(s_ClearColor.r, s_ClearColor.g, s_ClearColor.b, s_ClearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Render::EnableDepthTest(bool enable /*= true*/)
	{
		if (enable)
			glEnable(GL_DEPTH_TEST);
		else 
			glDisable(GL_DEPTH_TEST);
	}

	void Render::EnableBlending(bool enable /*= true*/)
	{
		if (enable)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else
			glDisable(GL_BLEND);
	}

	void Render::EnableCulling(bool enable /*= true*/, CullFace cullFace /*= CullFace::Front*/, FrontFace frontFace /*= FrontFace::Clockwise*/)
	{
		if (enable)
		{
			glEnable(GL_CULL_FACE);
			glCullFace((GLenum)cullFace);
			glFrontFace((GLenum)frontFace);
		}
		else
			glDisable(GL_CULL_FACE);
	}

	void Render::Viewport(float x, float y, float width, float height)
	{
		glViewport(x, y, width, height);
	}

	void Render::Viewport(float width, float height)
	{
		glViewport(0, 0, width, height);
	}

	void Render::Viewport(const mml::vec2& size)
	{
		glViewport(0, 0, size.x, size.y);
	}

	void Render::Viewport(const mml::vec4& viewport)
	{
		glViewport(viewport.x, viewport.y, viewport.z, viewport.w);
	}
}