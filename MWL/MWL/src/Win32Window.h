#pragma once

#include "Window.h"

#include <windows.h>
#include <vector>
#include <filesystem>
#include <cstdlib>

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

		void SetCursor(const Cursor& cursor) override;

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

		friend void SpawnWindowResizeEvent			(const Win32Window* window);
		friend void SpawnMouseMoveEvent				(const Win32Window* window);
		friend void SpawnMouseButtonDownEvent		(const Win32Window* window, MouseButton button);
		friend void SpawnMouseButtonUpEvent			(const Win32Window* window, MouseButton button);
		friend void SpawnMouseButtonClickedEvent	(const Win32Window* window, MouseButton button);
		friend void SpawnMouseScrollEvent			(const Win32Window* window, int delta);
		friend void SpawnKeyDownEvent				(const Win32Window* window, KeyCode keyCode, bool repeat);
		friend void SpawnKeyUpEvent					(const Win32Window* window, KeyCode keyCode);

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

		friend bool UpdateWindowMousePosition		(Win32Window* window, LPARAM lParam);

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
				HCURSOR LeftPointer			= nullptr;
				HCURSOR HandPointer			= nullptr;
				HCURSOR Text				= nullptr;
				HCURSOR ResizeHorizontal	= nullptr;
				HCURSOR ResizeVertical		= nullptr;
				HCURSOR ResizeBDiag			= nullptr;
				HCURSOR ResizeFDiag			= nullptr;
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

		struct State
		{
			std::wstring Title;
			
			uint32_t Width;
			uint32_t Height;

			uint32_t MouseX;
			uint32_t MouseY;
			MouseButton MouseClicked;

			bool VSync;
			bool Closed;
		};

		friend Win32Window::Titlebar::ButtonRects win32_get_title_bar_button_rects(HWND handle, const RECT* title_bar_rect);

		State		m_State;
		Win32Data	m_Win32Data;

		Titlebar	m_Titlebar;
		Style		m_Style;
	};
}