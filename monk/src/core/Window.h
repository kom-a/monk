#pragma once

#include <windows.h>
#include <string>
#include <functional>
#include <unordered_map>

#include "math/vec2.h"
#include "events/Event.h"

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
		void PollEvents();

		inline void Close() { m_WindowData.Closed = true; }
		inline bool Closed() const { return m_WindowData.Closed; }
		inline int GetWidth() const { return m_WindowData.Width; }
		inline int GetHeight() const { return m_WindowData.Height; }
		inline math::vec2 GetSize() const { return math::vec2((float)m_WindowData.Width, (float)m_WindowData.Height); }

		inline void SetEventCallback(std::function<void(Event&)> callback) {
			m_WindowData.EventCallbackFn = callback;
		}

		inline void SwapInterval(int interval) { m_SwapIntervalFn(interval); }

		void HideCursor(bool hide);
		void LockCursor(bool lock);

	private:
		friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		void ClipCursorInsideWindow(bool enable);
		void SetCursorPosition(int x, int y);

		bool CreateWin32Window();
		bool CreateOpenGLContext(int major, int minor);
		bool InitOpenGLContext();
		HWND CreateDummyWindow();
		void LoadOpenGLExtensions();

	private:
		static Window* GetWindowByHandle(const HWND& handle);

	private:
		static std::unordered_map<HWND, Window*> m_WindowTable;

		HWND m_WindowHandle;
		HGLRC m_OpenGLRenderingContext;
		
		struct WindowData
		{
			bool Closed;
			std::string Title;
			int Width;
			int Height;
			std::function<void(Event&)> EventCallbackFn;
		} m_WindowData;

		math::vec2 m_LockMousePosition;
		bool m_MouseLocked = false;

		PFNWGLSWAPINTERVALEXTPROC m_SwapIntervalFn;
	};
}