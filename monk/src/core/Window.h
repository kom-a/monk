#pragma once

#include <windows.h>
#include <string>
#include <functional>
#include <unordered_map>

namespace monk
{
	class Window
	{
	public:
		Window(int width, int height, const std::string& title);
		~Window();

	public:
		void Update();

		inline void Close() { m_WindowData.Closed = true; }
		inline bool Closed() const { return m_WindowData.Closed; }

	private:
		friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		static Window* GetWindowByHandle(const HWND& handle);

	private:
		static std::unordered_map<HWND, Window*> m_WindowTable;

		HWND m_WindowHandle;
		
		struct WindowData
		{
			bool Closed;
			bool Initialized;
			std::string Title;
			int Width;
			int Height;
		} m_WindowData;
	};
}