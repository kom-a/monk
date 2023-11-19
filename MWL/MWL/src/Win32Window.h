#pragma once

#include "Window.h"

#include <windows.h>

namespace mwl
{
	class Win32Window : public Window
	{
	public:
		Win32Window(const WindowProps& windowProps);
		~Win32Window();

	public:
		virtual void Update() override;
		virtual void Close() override;
		virtual void EnableVSync() override;

		virtual bool Closed() const override;
		virtual uint32_t GetWidth()	const override;
		virtual uint32_t GetHeight() const override;

	private:
		bool CreateWin32Window();
		bool CreateOpenGLContext(int major, int minor);
		bool InitOpenGLContext();
		HWND CreateDummyWindow();
		void LoadOpenGLExtensions();

		friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		struct WindowData
		{
			bool Closed;
			std::wstring Title;
			uint32_t Width;
			uint32_t Height;
		} m_WindowData;

		struct Win32Data
		{
			HWND WindowHandle;
			HGLRC OpenGLRenderingContext;
		} m_Win32Data;
		
	};
}