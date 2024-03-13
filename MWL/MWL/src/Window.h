#pragma once

#include <string>
#include <inttypes.h>

namespace mwl
{
	struct WindowProps
	{
		std::wstring Title	= L"MWL Window";
		uint32_t Width		= 1600;
		uint32_t Height		= 900;
		bool VSync			= true;
		bool Closed			= false;
	};

	enum class OpenGLVersion
	{
		OPENGL_1_0,
		OPENGL_1_1,
		OPENGL_1_2,
		OPENGL_1_3,
		OPENGL_1_4,
		OPENGL_1_5,
		OPENGL_2_0,
		OPENGL_2_1,
		OPENGL_3_0,
		OPENGL_3_1,
		OPENGL_3_2,
		OPENGL_3_3,
		OPENGL_4_0,
		OPENGL_4_1,
		OPENGL_4_2,
		OPENGL_4_3,
		OPENGL_4_4,
		OPENGL_4_5,
		OPENGL_4_6
	};

	void SetOpenGLVersion(OpenGLVersion openglVersion);

	class Window
	{
	public:
		virtual ~Window() = default;

	public:
		virtual void Update()					= 0;
		virtual void Close()					= 0;
		virtual void EnableVSync(bool enable)	= 0;

		virtual bool Closed()			const = 0;
		virtual uint32_t GetWidth()		const = 0;
		virtual uint32_t GetHeight()	const = 0;
	};

	Window* Create(const WindowProps& windowProps = WindowProps());
}