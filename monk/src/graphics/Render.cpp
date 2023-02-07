#include "Render.h"

#include "utils/OpenGL.h"

namespace monk
{
	math::vec4 Render::s_ClearColor = math::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	void Render::SetClearColor(const math::vec4& clearColor)
	{
		s_ClearColor = clearColor;
	}

	void Render::Clear()
	{
		glClearColor(s_ClearColor.r, s_ClearColor.g, s_ClearColor.b, s_ClearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
}