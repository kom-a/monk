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