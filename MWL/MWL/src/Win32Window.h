#pragma once

#include "Window.h"

#include <windows.h>
#include <vector>

namespace mwl
{
	class Win32Window : public Window
	{
	public:
		explicit Win32Window(const WindowProps& windowProps);
		~Win32Window() override = default;

	public:
		void Update() override;
		void Close() override;
		void EnableVSync(bool enable) override;

		bool Closed() const override;
		uint32_t GetWidth()	const override;
		uint32_t GetHeight() const override;

	private:
		bool CreateWin32Window();
		bool CreateOpenGLContext(OpenGLVersion openglVerson);
		bool InitOpenGLContext();
		HWND CreateDummyWindow();
		void LoadOpenGLExtensions() const;
		HWND CreateOpenGLHandle();
		std::vector<int> GetPixelFormatAttribs() const;
		std::vector<int> GetOpenGLContextAttribs(OpenGLVersion openglVerson) const;
		bool SetOpenGLPixelFormat(HWND handle);
		HGLRC CreateOpenGLRenderingContext(OpenGLVersion openglVerson);

		friend RECT Win32GetTitlebarRect			(HWND handle);

		friend LRESULT Win32HandleWM_NCCALCSIZE		(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
		friend LRESULT Win32HandleWM_CREATE			(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
		friend LRESULT Win32HandleWM_ACTIVATE		(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
		friend LRESULT Win32HandleWM_NCHITTEST		(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
		friend LRESULT Win32HandleWM_PAINT			(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
		friend LRESULT Win32HandleWM_SIZE			(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
		friend LRESULT Win32HandleWM_NCMOUSEMOVE	(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
		friend LRESULT Win32HandleWM_MOUSEMOVE		(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
		friend LRESULT Win32HandleWM_NCLBUTTONDOWN	(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
		friend LRESULT Win32HandleWM_NCLBUTTONUP	(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);
		friend LRESULT Win32HandleWM_SETCURSOR		(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam);

		friend LRESULT CALLBACK WindowProc			(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		friend LRESULT CALLBACK OpenGLPanelProc		(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		struct Win32Data
		{
			HWND WindowHandle;
			HWND OpenGLHandle;
			HGLRC OpenGLRenderingContext;
		};

		struct Style
		{
			struct
			{
				COLORREF Background = RGB(30, 30, 35);
				COLORREF Titlebar	= RGB(30, 30, 35);
			} Colors;

			struct
			{
				HCURSOR LeftPointer			= LoadCursorFromFile(L"res/oxy-bluecurve/left_ptr.cur");
				HCURSOR HandPointer			= LoadCursorFromFile(L"res/oxy-bluecurve/pointing_hand.cur");
				HCURSOR Text				= LoadCursorFromFile(L"res/oxy-bluecurve/Text.cur");
				HCURSOR ResizeHorizontal	= LoadCursorFromFile(L"res/oxy-bluecurve/size_hor.cur");
				HCURSOR ResizeVertical		= LoadCursorFromFile(L"res/oxy-bluecurve/size_ver.cur");
				HCURSOR ResizeBDiag			= LoadCursorFromFile(L"res/oxy-bluecurve/size_bdiag.cur");
				HCURSOR ResizeFDiag			= LoadCursorFromFile(L"res/oxy-bluecurve/size_fdiag.cur");
			} Cursor;
		};

		struct Titlebar
		{
			struct ButtonRects
			{
				RECT Close;
				RECT Maximize;
				RECT Minimize;
			};

			enum class HoveredButton
			{
				None,
				Close,
				Maximize,
				Minimize
			};

			uint32_t Height = 25;
			HoveredButton CurrentHoveredButton = HoveredButton::None;
		};

		friend Win32Window::Titlebar::ButtonRects win32_get_title_bar_button_rects(HWND handle, const RECT* title_bar_rect);

		WindowProps m_WindowData;
		Win32Data	m_Win32Data;

		Titlebar	m_Titlebar;
		Style		m_Style;
	};
}