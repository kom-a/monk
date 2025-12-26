#pragma once

#include <MWL/Window.h>

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
		void SwapBuffers() override;
		void Close() override;
		void EnableVSync(bool enable) override;
		void MakeContextCurrent() override;

		bool Closed() const override;
		bool IsFullscreen() const override;
		uint32_t GetWidth()	const override;
		uint32_t GetHeight() const override;
		float GetAspectRatio() const override;
		void* GetNative() override;
		WindowNativeType GetNativeType() const override;

		KeyCodeState GetKeyCodeState(KeyCode keyCode) const override;

		void LoadCursorData(const CursorData& cursor) override;
		void SetCursor(CursorType cursorType) override;

		void SetFullscreen(bool fullscreen) override;

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

		HCURSOR TryLoadCursorFromFile(const std::wstring& path, LPCWSTR defaultCursor);

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
				COLORREF Background				= RGB(30, 30, 35);
				COLORREF Titlebar				= RGB(20, 20, 30);
				COLORREF TitlebarButtonHover	= RGB(10, 10, 20);
				COLORREF TitlebarItem			= RGB(255, 255, 255);
				COLORREF TitlebarItemNoFocus	= RGB(100, 100, 100);
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
			} Cursors;
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

			uint32_t Height = 24;
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

			std::vector<KeyCodeState> KeyCodeStates;

			OpenGLVersion OpenGLContextVersion;

			bool VSync;
			bool Closed;

			bool IsFullscreen;
			WINDOWPLACEMENT FullscreenRecoverPlacement;
		};

		friend Win32Window::Titlebar::ButtonRects Win32GetTitlebarButtonRects(HWND handle, const RECT* title_bar_rect);

		State		m_State;
		Win32Data	m_Win32Data;

		Titlebar	m_Titlebar;
		Style		m_Style;
	};
}