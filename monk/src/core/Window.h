#pragma once

#include <windows.h>
#include <string>
#include <functional>
#include <unordered_map>

#include "math/vec2.h"

// wgl extension pointer typedefs
typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int);

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
		inline int GetWidth() const { return m_WindowData.Width; }
		inline int GetHeight() const { return m_WindowData.Height; }
		inline math::vec2 GetSize() const { return math::vec2((float)m_WindowData.Width, (float)m_WindowData.Height); }

		inline void SwapInterval(int interval) { m_SwapIntervalFn(interval); }

	private:
		friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		bool CreateWin32Window();
		bool CreateOpenGLContext();

	private:
		static Window* GetWindowByHandle(const HWND& handle);

	private:
		static std::unordered_map<HWND, Window*> m_WindowTable;

		HWND m_WindowHandle;
		
		struct WindowData
		{
			bool Closed;
			std::string Title;
			int Width;
			int Height;
		} m_WindowData;

		PFNWGLSWAPINTERVALEXTPROC m_SwapIntervalFn;
	};
}