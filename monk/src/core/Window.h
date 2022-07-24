#pragma once

#include <windows.h>
#include <string>

namespace monk
{
	class Window
	{
	public:
		Window(int width, int height, const std::string& title);
		~Window();

		void Update();

	private:
		const std::string m_WindowClassName = "MonkWindowClassName";

		HWND m_WindowHandle;
	};
}