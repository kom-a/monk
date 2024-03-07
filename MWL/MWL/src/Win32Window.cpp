#include "Win32Window.h"

#include <uxtheme.h>
#include <vssym32.h>
#include <windowsx.h>

#include "Log.h"

#define UNUSED(name) (void)name

constexpr auto GL_FALSE	= 0;
constexpr auto GL_TRUE	= 1;

constexpr auto WGL_DRAW_TO_WINDOW_ARB			= 0x2001;
constexpr auto WGL_ACCELERATION_ARB				= 0x2003;
constexpr auto WGL_SUPPORT_OPENGL_ARB			= 0x2010;
constexpr auto WGL_DOUBLE_BUFFER_ARB			= 0x2011;
constexpr auto WGL_PIXEL_TYPE_ARB				= 0x2013;
constexpr auto WGL_COLOR_BITS_ARB				= 0x2014;
constexpr auto WGL_DEPTH_BITS_ARB				= 0x2022;
constexpr auto WGL_STENCIL_BITS_ARB				= 0x2023;
constexpr auto WGL_FULL_ACCELERATION_ARB		= 0x2027;
constexpr auto WGL_TYPE_RGBA_ARB				= 0x202B;
constexpr auto WGL_CONTEXT_MAJOR_VERSION_ARB	= 0x2091;
constexpr auto WGL_CONTEXT_MINOR_VERSION_ARB	= 0x2092;
constexpr auto WGL_CONTEXT_PROFILE_MASK_ARB		= 0x9126;
constexpr auto WGL_CONTEXT_CORE_PROFILE_BIT_ARB = 0x00000001;

typedef BOOL(WINAPI* PFNWGLCHOOSEPIXELFOTMATARBPROC)(HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
PFNWGLCHOOSEPIXELFOTMATARBPROC wglChoosePixelFormatARB;

typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hdc, HGLRC hShareContext, const int* attribList);
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int interval);
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

namespace mwl
{
	Window* Create(const WindowProps& windowProps /*= WindowProps()*/)
	{
		return new Win32Window(windowProps);
	}

	Win32Window::Win32Window(const WindowProps& windowProps)
		: m_WindowData(windowProps)
	{
		if (!CreateWin32Window())
		{
			LOG_CRITICAL("Could not create window");
			Close();
		}
		else
			LOG_INFO("Window successfully created");

		if (!CreateOpenGLContext(3, 3))
		{
			LOG_CRITICAL("Could not create OpenGL context");
			Close();
		}
		else
			LOG_INFO("OpenGL context successfully created");

		ShowWindow(m_Win32Data.WindowHandle, SW_SHOW);
		EnableVSync(m_WindowData.VSync);

		SetWindowLongPtr(m_Win32Data.WindowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	}

	void Win32Window::Update()
	{
		MSG msg;

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		SwapBuffers(GetDC(m_Win32Data.OpenGLHandle));
	}

	void Win32Window::Close()
	{
		m_WindowData.Closed = true;
	}

	void Win32Window::EnableVSync(bool enable)
	{
		m_WindowData.VSync = enable;

		int interval = enable ? 1 : 0;
		wglSwapIntervalEXT(interval);
	}

	bool Win32Window::Closed() const
	{
		return m_WindowData.Closed;
	}

	uint32_t Win32Window::GetWidth() const
	{
		return m_WindowData.Width;
	}

	uint32_t Win32Window::GetHeight() const
	{
		return m_WindowData.Height - m_Titlebar.Height;
	}

	bool Win32Window::CreateWin32Window()
	{
		WNDCLASSEX wc		= { 0 };
		wc.cbSize			= sizeof(wc);
		wc.style			= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc		= WindowProc;
		wc.hInstance		= GetModuleHandle(nullptr);
		wc.hCursor			= LoadCursor(nullptr, IDC_ARROW);
		wc.lpszClassName	= m_WindowData.Title.c_str();

		if (!RegisterClassEx(&wc))
		{
			LOG_ERROR("Could not register '{0}' window class", m_WindowData.Title);
			return false;
		}
		else 
			LOG_INFO("Window class '{0}' registered", m_WindowData.Title);

		m_Win32Data.WindowHandle = CreateWindowEx(WS_EX_LEFT,
			m_WindowData.Title.c_str(),
			m_WindowData.Title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			m_WindowData.Width,
			m_WindowData.Height,
			nullptr,
			nullptr,
			GetModuleHandle(nullptr),
			nullptr);

		RECT clientRect;
		GetClientRect(m_Win32Data.WindowHandle, &clientRect);
		m_WindowData.Width = clientRect.right - clientRect.left;
		m_WindowData.Height = clientRect.bottom - clientRect.top;

		return m_Win32Data.WindowHandle != nullptr;
	}

	bool Win32Window::CreateOpenGLContext(int major, int minor)
	{
		if (!InitOpenGLContext())
		{
			LOG_ERROR("Failed to init dummy OpenGL context");
			return false;
		}

		m_Win32Data.OpenGLHandle = CreateOpenGLHandle();
		SetWindowLongPtr(m_Win32Data.OpenGLHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

		if (!m_Win32Data.OpenGLHandle)
		{
			LOG_ERROR("Failed to create OpenGL panel");
			return false;
		}

		if (!SetOpenGLPixelFormat(m_Win32Data.OpenGLHandle))
		{
			LOG_ERROR("Failed to set OpenGL pixel format");
			return false;
		}

		m_Win32Data.OpenGLRenderingContext = CreateOpenGLRenderingContext(major, minor);

		if (!m_Win32Data.OpenGLRenderingContext)
		{
			LOG_ERROR("Failed to create OpenGL rendering context");
			return false;
		}

		return true;
	}

	bool Win32Window::InitOpenGLContext()
	{
		HWND dummyWindow = CreateDummyWindow();
		HDC dummyDC = GetDC(dummyWindow);

		HGLRC dummyContext = wglCreateContext(dummyDC);

		if (!dummyContext)
		{
			LOG_ERROR("Failed to create a dummy OpenGL rendering context.");
			wglMakeCurrent(dummyDC, nullptr);
			wglDeleteContext(dummyContext);
			ReleaseDC(dummyWindow, dummyDC);
			DestroyWindow(dummyWindow);
			return false;
		}

		if (!wglMakeCurrent(dummyDC, dummyContext))
		{
			LOG_ERROR("Failed to activate dummy OpenGL rendering context.");
			wglMakeCurrent(dummyDC, nullptr);
			wglDeleteContext(dummyContext);
			ReleaseDC(dummyWindow, dummyDC);
			DestroyWindow(dummyWindow);
			return false;
		}

		LoadOpenGLExtensions();

		wglMakeCurrent(dummyDC, nullptr);
		wglDeleteContext(dummyContext);
		ReleaseDC(dummyWindow, dummyDC);
		DestroyWindow(dummyWindow);

		return true;
	}

	HWND Win32Window::CreateDummyWindow()
	{
		WNDCLASSA wc		= { 0 };
		wc.style			= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc		= DefWindowProcA;
		wc.hInstance		= GetModuleHandle(nullptr);
		wc.lpszClassName	= "MWL_WGL_DUMMY_WINDOW";

		if (!RegisterClassA(&wc))
			LOG_ERROR("Failed to register dummy OpenGL window: '{0}'", wc.lpszClassName);
		else
			LOG_INFO("Dummy OpenGL context successfully created: '{0}'", wc.lpszClassName);

		HWND window = CreateWindowExA(
			0,
			wc.lpszClassName,
			"Dummy OpenGL Window",
			0,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			nullptr,
			nullptr,
			wc.hInstance,
			nullptr);

		if (!window)
			LOG_ERROR("Failed to create dummy OpenGL window");
		else
			LOG_INFO("Dummy OpenGL window successfully created");

		HDC dc = GetDC(window);

		PIXELFORMATDESCRIPTOR pfd	= { 0 };
		pfd.nSize					= sizeof(pfd);
		pfd.nVersion				= 1;
		pfd.iPixelType				= PFD_TYPE_RGBA;
		pfd.dwFlags					= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.cColorBits				= 32;
		pfd.cAlphaBits				= 8;
		pfd.iLayerType				= PFD_MAIN_PLANE;
		pfd.cDepthBits				= 24;
		pfd.cStencilBits			= 8;

		int pixelFormat = ChoosePixelFormat(dc, &pfd);

		if (!pixelFormat)
			LOG_ERROR("Failed to find a suitable pixel format.");

		if (!SetPixelFormat(dc, pixelFormat, &pfd))
			LOG_ERROR("Failed to set the pixel format.");

		return window;
	}

	void Win32Window::LoadOpenGLExtensions() const
	{
		wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFOTMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	}

	HWND Win32Window::CreateOpenGLHandle()
	{
		std::wstring opengl_panel_name = L"__OpenGLPanel__";

		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = OpenGLPanelProc;
		wc.hInstance = GetModuleHandle(nullptr);
		wc.hCursor = LoadCursorFromFile(L"cross.cur");
		wc.lpszClassName = opengl_panel_name.c_str();

		if (!RegisterClassEx(&wc))
		{
			LOG_ERROR("Could not register '{0}' class", opengl_panel_name.c_str());
			return nullptr;
		}
		else
			LOG_INFO("Window class '{0}' registered", opengl_panel_name.c_str());

		HWND opengl_panel_handle = CreateWindowEx(WS_EX_LEFT,
			opengl_panel_name.c_str(),
			opengl_panel_name.c_str(),
			WS_CHILD | WS_VISIBLE | CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
			0,
			m_Titlebar.Height,
			m_WindowData.Width,
			m_WindowData.Height - m_Titlebar.Height,
			m_Win32Data.WindowHandle,
			nullptr,
			GetModuleHandle(nullptr),
			nullptr);

		return opengl_panel_handle;
	}

	std::vector<int> Win32Window::GetPixelFormatAttribs() const
	{
		std::vector<int> pixelFormatAttribs = {
			WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
			WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
			WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,         32,
			WGL_DEPTH_BITS_ARB,         24,
			WGL_STENCIL_BITS_ARB,       8,
			0
		};
		
		return pixelFormatAttribs;
	}

	std::vector<int> Win32Window::GetOpenGLContextAttribs(int major, int minor) const
	{
		std::vector<int> glAttribs = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, major,
			WGL_CONTEXT_MINOR_VERSION_ARB, minor,
			WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0,
		};

		return glAttribs;
	}

	bool Win32Window::SetOpenGLPixelFormat(HWND handle)
	{
		HDC dc = GetDC(handle);

		std::vector<int> pixel_format_attribs = GetPixelFormatAttribs();

		int pixel_format;
		UINT num_formats;
		wglChoosePixelFormatARB(dc, pixel_format_attribs.data(), nullptr, 1, &pixel_format, &num_formats);
		if (!num_formats)
			return false;

		PIXELFORMATDESCRIPTOR pfd;
		DescribePixelFormat(dc, pixel_format, sizeof(pfd), &pfd);
		if (!SetPixelFormat(dc, pixel_format, &pfd))
			return false;

		return true;
	}

	HGLRC Win32Window::CreateOpenGLRenderingContext(int major, int minor)
	{
		std::vector<int> gl_attribs = GetOpenGLContextAttribs(major, minor);

		HGLRC opengl_rendering_context = wglCreateContextAttribsARB(GetDC(m_Win32Data.OpenGLHandle), nullptr, gl_attribs.data());
		if (!opengl_rendering_context)
		{
			LOG_CRITICAL("Failed to create OpenGL context.");
			return nullptr;
		}
		
		if (!wglMakeCurrent(GetDC(m_Win32Data.OpenGLHandle), opengl_rendering_context))
		{
			LOG_CRITICAL("Failed to activate OpenGL rendering context.");
			return nullptr;
		}
		
		return opengl_rendering_context;
	}

	static int Win32DpiScale(int value, UINT dpi)
	{
		return (int)((float)value * (float)dpi / 96.0f);
	}

	static Win32Window::Titlebar::ButtonRects win32_get_title_bar_button_rects(HWND handle, const RECT* title_bar_rect) 
	{
		auto window = (Win32Window*)GetWindowLongPtr(handle, GWLP_USERDATA);
		if (!window)
			return { 0 };

		UINT dpi = GetDpiForWindow(handle);
		Win32Window::Titlebar::ButtonRects button_rects;

		// Sadly SM_CXSIZE does not result in the right size buttons for Win10
		int button_width = Win32DpiScale(window->m_Titlebar.Height, dpi);
		button_rects.Close = *title_bar_rect;

		button_rects.Close.left = button_rects.Close.right - button_width;
		button_rects.Maximize = button_rects.Close;
		button_rects.Maximize.left -= button_width;
		button_rects.Maximize.right -= button_width;
		button_rects.Minimize = button_rects.Maximize;
		button_rects.Minimize.left -= button_width;
		button_rects.Minimize.right -= button_width;
		return button_rects;
	}

	static RECT Win32GetTitlebarRect(HWND handle) 
	{
#if 0
		SIZE title_bar_size = { 0 };
		HTHEME theme = OpenThemeData(handle, L"WINDOW");
		UINT dpi = GetDpiForWindow(handle);
		GetThemePartSize(theme, nullptr, WP_CAPTION, CS_ACTIVE, nullptr, TS_TRUE, &title_bar_size);
		CloseThemeData(theme);
#endif 
		auto window = (Win32Window*)GetWindowLongPtr(handle, GWLP_USERDATA);
		if (!window)
			return { 0 };

		UINT dpi = GetDpiForWindow(handle);
		int height = Win32DpiScale(window->m_Titlebar.Height, dpi);
		
		RECT rect;
		GetClientRect(handle, &rect);
		rect.bottom = rect.top + height;
		return rect;
	}

	static void win32_center_rect_in_rect(RECT* to_center,const RECT* outer_rect) 
	{
		int to_width = to_center->right - to_center->left;
		int to_height = to_center->bottom - to_center->top;
		int outer_width = outer_rect->right - outer_rect->left;
		int outer_height = outer_rect->bottom - outer_rect->top;

		int padding_x = (outer_width - to_width) / 2;
		int padding_y = (outer_height - to_height) / 2;

		to_center->left = outer_rect->left + padding_x;
		to_center->top = outer_rect->top + padding_y;
		to_center->right = to_center->left + to_width;
		to_center->bottom = to_center->top + to_height;
	}

	static bool Win32IsWindowMaximized(HWND hWindow)
	{
		WINDOWPLACEMENT placement	= { 0 };
		placement.length			= sizeof(WINDOWPLACEMENT);

		if (GetWindowPlacement(hWindow, &placement))
			return placement.showCmd == SW_SHOWMAXIMIZED;

		return false;
	}

	static LRESULT Win32HandleWM_NCCALCSIZE(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
	{
		// Handling this event allows us to extend client (paintable) area into the title bar region
		// The information is partially coming from:
		// https://docs.microsoft.com/en-us/windows/win32/dwm/customframe#extending-the-client-frame
		// Most important paragraph is:
		//   To remove the standard window frame, you must handle the WM_NCCALCSIZE message,
		//   specifically when its wParam value is TRUE and the return value is 0.
		//   By doing so, your application uses the entire window region as the client area,
		//   removing the standard frame.

		if (!wParam)
			return DefWindowProc(hWindow, uMessage, wParam, lParam);

		UINT dpi = GetDpiForWindow(hWindow);

		int frame_x = GetSystemMetricsForDpi(SM_CXFRAME, dpi);
		int frame_y = GetSystemMetricsForDpi(SM_CYFRAME, dpi);
		int padding = GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi);

		auto* params = (NCCALCSIZE_PARAMS*)lParam;
		RECT* requested_client_rect = params->rgrc;

		requested_client_rect->right -= frame_x + padding;
		requested_client_rect->left += frame_x + padding;
		requested_client_rect->bottom -= frame_y + padding;

		if (Win32IsWindowMaximized(hWindow))
			requested_client_rect->top += padding;

		return 0;
	}

	static LRESULT Win32HandleWM_CREATE(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
	{
		UNUSED(uMessage);
		UNUSED(wParam);
		UNUSED(lParam);

		RECT size_rect;
		GetWindowRect(hWindow, &size_rect);

		// Inform the application of the frame change to force redrawing with the new
		// client area that is extended into the title bar
		SetWindowPos(
			hWindow, nullptr,
			size_rect.left, size_rect.top,
			size_rect.right - size_rect.left, size_rect.bottom - size_rect.top,
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE
		);

		return DefWindowProc(hWindow, uMessage, wParam,	lParam);
	}

	static LRESULT Win32HandleWM_ACTIVATE(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
	{
		RECT title_bar_rect = Win32GetTitlebarRect(hWindow);
		InvalidateRect(hWindow, &title_bar_rect, FALSE);
		return DefWindowProc(hWindow, uMessage, wParam, lParam);
	}

	static LRESULT Win32HandleWM_NCHITTEST(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
	{
		// Let the default procedure handle resizing areas
		LRESULT hit = DefWindowProc(hWindow, uMessage, wParam, lParam);
		switch (hit)
		{
			case HTNOWHERE:
			case HTRIGHT:
			case HTLEFT:
			case HTTOPLEFT:
			case HTTOP:
			case HTTOPRIGHT:
			case HTBOTTOMRIGHT:
			case HTBOTTOM:
			case HTBOTTOMLEFT:
				return hit;
		}

		auto window = (Win32Window*)GetWindowLongPtr(hWindow, GWLP_USERDATA);
		if (!window)
			return DefWindowProc(hWindow, uMessage, wParam, lParam);

		Win32Window::Titlebar::HoveredButton title_bar_hovered_button = window->m_Titlebar.CurrentHoveredButton;

		// Check if hover button is on maximize to support SnapLayout on Windows 11
		if (title_bar_hovered_button == Win32Window::Titlebar::HoveredButton::Maximize)
			return HTMAXBUTTON;

		// Looks like adjustment happening in NCCALCSIZE is messing with the detection
		// of the top hit area so manually fixing that.
		UINT dpi = GetDpiForWindow(hWindow);
		int frame_y = GetSystemMetricsForDpi(SM_CYFRAME, dpi);
		int padding = GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi);
		POINT cursor_point = { 0 };
		cursor_point.x = GET_X_LPARAM(lParam);
		cursor_point.y = GET_Y_LPARAM(lParam);
		ScreenToClient(hWindow, &cursor_point);
		if (cursor_point.y > 0 && cursor_point.y < frame_y + padding)
			return HTTOP;

		// Since we are drawing our own caption, this needs to be a custom test
		if (cursor_point.y < Win32GetTitlebarRect(hWindow).bottom)
			return HTCAPTION;

		return HTCLIENT;
	}

	static LRESULT Win32HandleWM_PAINT(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
	{
		UNUSED(uMessage);
		UNUSED(wParam);
		UNUSED(lParam);

		auto window = (const Win32Window*)GetWindowLongPtr(hWindow, GWLP_USERDATA);
		if (!window)
			return DefWindowProc(hWindow, uMessage, wParam, lParam);

		const Win32Window::Style& style = window->m_Style;

		bool has_focus = !!GetFocus();

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWindow, &ps);

		// Paint Background
		auto bg_color = style.Colors.Background;
		HBRUSH bg_brush = CreateSolidBrush(bg_color);
		FillRect(hdc, &ps.rcPaint, bg_brush);
		DeleteObject(bg_brush);

		// Paint Title Bar
		HTHEME theme = OpenThemeData(hWindow, L"WINDOW");

		const auto& title_bar_color = style.Colors.Titlebar;
		HBRUSH title_bar_brush = CreateSolidBrush(title_bar_color);
		COLORREF title_bar_hover_color = RGB(45, 45, 50);
		HBRUSH title_bar_hover_brush = CreateSolidBrush(title_bar_hover_color);

		RECT title_bar_rect = Win32GetTitlebarRect(hWindow);

		// Title Bar Background
		FillRect(hdc, &title_bar_rect, title_bar_brush);

		COLORREF title_bar_item_color = has_focus ? RGB(220, 220, 220) : RGB(127, 127, 127);

		HBRUSH button_icon_brush = CreateSolidBrush(title_bar_item_color);
		HPEN button_icon_pen = CreatePen(PS_SOLID, 1, title_bar_item_color);

		Win32Window::Titlebar::ButtonRects button_rects = win32_get_title_bar_button_rects(hWindow, &title_bar_rect);

		UINT dpi = GetDpiForWindow(hWindow);
		int icon_dimension = Win32DpiScale(9, dpi);
		
		const auto title_bar_hovered_button = window->m_Titlebar.CurrentHoveredButton;

		{ 
			// Minimize button
			bool const is_hovered = title_bar_hovered_button == Win32Window::Titlebar::HoveredButton::Minimize;
			if (title_bar_hovered_button == Win32Window::Titlebar::HoveredButton::Minimize) {
				FillRect(hdc, &button_rects.Minimize, title_bar_hover_brush);
			}
			RECT icon_rect = { 0 };
			icon_rect.right = icon_dimension;
			icon_rect.bottom = 1;
			win32_center_rect_in_rect(&icon_rect, &button_rects.Minimize);
			FillRect(hdc, &icon_rect, button_icon_brush);
		}

		{ 
			// Maximize button
			bool const is_hovered = title_bar_hovered_button == Win32Window::Titlebar::HoveredButton::Maximize;
			if (is_hovered) {
				FillRect(hdc, &button_rects.Maximize, title_bar_hover_brush);
			}
			RECT icon_rect = { 0 };
			icon_rect.right = icon_dimension;
			icon_rect.bottom = icon_dimension;
			win32_center_rect_in_rect(&icon_rect, &button_rects.Maximize);
			SelectObject(hdc, button_icon_pen);
			SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
			Rectangle(hdc, icon_rect.left, icon_rect.top, icon_rect.right, icon_rect.bottom);
		}

		{ 
			// Close button
			HPEN custom_pen = 0;
			bool const is_hovered = title_bar_hovered_button == Win32Window::Titlebar::HoveredButton::Close;
			if (is_hovered) {
				HBRUSH fill_brush = CreateSolidBrush(RGB(0xCC, 0, 0));
				FillRect(hdc, &button_rects.Close, fill_brush);
				DeleteObject(fill_brush);
				custom_pen = CreatePen(PS_SOLID, 1, RGB(0xFF, 0xFF, 0xFF));
				SelectObject(hdc, custom_pen);
			}
			RECT icon_rect = { 0 };
			icon_rect.right = icon_dimension;
			icon_rect.bottom = icon_dimension;
			win32_center_rect_in_rect(&icon_rect, &button_rects.Close);
			MoveToEx(hdc, icon_rect.left, icon_rect.top, NULL);
			LineTo(hdc, icon_rect.right + 1, icon_rect.bottom + 1);
			MoveToEx(hdc, icon_rect.left, icon_rect.bottom, NULL);
			LineTo(hdc, icon_rect.right + 1, icon_rect.top - 1);
			if (custom_pen) DeleteObject(custom_pen);
		}
		DeleteObject(title_bar_hover_brush);
		DeleteObject(button_icon_brush);
		DeleteObject(button_icon_pen);
		DeleteObject(title_bar_brush);

		// Draw window title
		LOGFONT logical_font;
		HFONT old_font = NULL;
		if (SUCCEEDED(SystemParametersInfoForDpi(SPI_GETICONTITLELOGFONT, sizeof(logical_font), &logical_font, false, dpi))) {
			HFONT theme_font = CreateFontIndirect(&logical_font);
			old_font = (HFONT)SelectObject(hdc, theme_font);
		}

		wchar_t title_text_buffer[255] = { 0 };
		int buffer_length = sizeof(title_text_buffer) / sizeof(title_text_buffer[0]);
		GetWindowText(hWindow, title_text_buffer, buffer_length);
		RECT title_bar_text_rect = title_bar_rect;
		// Add padding on the right for the buttons
		//title_bar_text_rect.right = button_rects.Minimize.left;
		DTTOPTS draw_theme_options = { sizeof(draw_theme_options) };
		draw_theme_options.dwFlags = DTT_TEXTCOLOR;
		draw_theme_options.crText = title_bar_item_color;
		DrawThemeTextEx(
			theme,
			hdc,
			0, 0,
			title_text_buffer,
			-1,
			DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS,
			&title_bar_text_rect,
			&draw_theme_options
		);
		if (old_font) SelectObject(hdc, old_font);
		CloseThemeData(theme);

		EndPaint(hWindow, &ps);

		return DefWindowProc(hWindow, uMessage, wParam, lParam);
	}

	static LRESULT Win32HandleWM_SIZE(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
	{
		auto window = (Win32Window*)GetWindowLongPtr(hWindow, GWLP_USERDATA);
		if (!window)
			return DefWindowProc(hWindow, uMessage, wParam, lParam);

		uint32_t width = LOWORD(lParam);
		uint32_t height = HIWORD(lParam);

		window->m_WindowData.Width = width;
		window->m_WindowData.Height = height;

		auto titlebar_height = window->m_Titlebar.Height;

		SetWindowPos(window->m_Win32Data.OpenGLHandle,
			nullptr,
			0,
			titlebar_height,
			width,
			height - titlebar_height,
			0);

		return DefWindowProc(hWindow, uMessage, wParam, lParam);
	}

	LRESULT Win32HandleWM_NCMOUSEMOVE(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
	{
		auto window = (Win32Window*)GetWindowLongPtr(hWindow, GWLP_USERDATA);
		if (!window)
			return DefWindowProc(hWindow, uMessage, wParam, lParam);

		auto title_bar_hovered_button = window->m_Titlebar.CurrentHoveredButton;

		// Track when mouse hovers each of the title bar buttons to draw the highlight correctly
		POINT cursor_point;
		GetCursorPos(&cursor_point);
		ScreenToClient(hWindow, &cursor_point);

		RECT title_bar_rect = Win32GetTitlebarRect(hWindow);
		Win32Window::Titlebar::ButtonRects button_rects = win32_get_title_bar_button_rects(hWindow, &title_bar_rect);

		Win32Window::Titlebar::HoveredButton new_hovered_button = Win32Window::Titlebar::HoveredButton::None;
		if (PtInRect(&button_rects.Close, cursor_point)) 
			new_hovered_button = Win32Window::Titlebar::HoveredButton::Close;
		else if (PtInRect(&button_rects.Minimize, cursor_point)) 
			new_hovered_button = Win32Window::Titlebar::HoveredButton::Minimize;
		else if (PtInRect(&button_rects.Maximize, cursor_point)) 
			new_hovered_button = Win32Window::Titlebar::HoveredButton::Maximize;

		if (new_hovered_button != title_bar_hovered_button) 
		{
			// You could do tighter invalidation here but probably doesn't matter
			InvalidateRect(hWindow, &button_rects.Close, FALSE);
			InvalidateRect(hWindow, &button_rects.Minimize, FALSE);
			InvalidateRect(hWindow, &button_rects.Maximize, FALSE);

			window->m_Titlebar.CurrentHoveredButton = new_hovered_button;
		}

		return DefWindowProc(hWindow, uMessage, wParam, lParam);
	}

	static LRESULT Win32HandleWM_MOUSEMOVE(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
	{
		auto window = (Win32Window*)GetWindowLongPtr(hWindow, GWLP_USERDATA);
		if (!window)
			return DefWindowProc(hWindow, uMessage, wParam, lParam);

		Win32Window::Titlebar::HoveredButton title_bar_hovered_button = window->m_Titlebar.CurrentHoveredButton;

		// If the mouse gets into the client area then no title bar buttons are hovered
		// so need to reset the hover state
		if (title_bar_hovered_button != Win32Window::Titlebar::HoveredButton::None)
		{
			RECT title_bar_rect = Win32GetTitlebarRect(hWindow);
			// You could do tighter invalidation here but probably doesn't matter
			InvalidateRect(hWindow, &title_bar_rect, FALSE);
			window->m_Titlebar.CurrentHoveredButton = Win32Window::Titlebar::HoveredButton::None;
		}

		return DefWindowProc(hWindow, uMessage, wParam, lParam);
	}

	static LRESULT Win32HandleWM_NCLBUTTONDOWN(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
	{
		auto window = (Win32Window*)GetWindowLongPtr(hWindow, GWLP_USERDATA);
		if (!window)
			return DefWindowProc(hWindow, uMessage, wParam, lParam);

		Win32Window::Titlebar::HoveredButton title_bar_hovered_button = window->m_Titlebar.CurrentHoveredButton;

		// handle mouse down and mouse up in the caption area to handle clicks on the buttons

		// Clicks on buttons will be handled in WM_NCLBUTTONUP, but we still need
		// to remove default handling of the click to avoid it counting as drag.
		//
		// Ideally you also want to check that the mouse hasn't moved out or too much
		// between DOWN and UP uMessages.
		if (title_bar_hovered_button != Win32Window::Titlebar::HoveredButton::None)
			return 0;
		// Default handling allows for dragging and double click to maximize
		return DefWindowProc(hWindow, uMessage, wParam, lParam);
	}

	static LRESULT Win32HandleWM_NCLBUTTONUP(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
	{
		auto window = (const Win32Window*)(GetWindowLongPtr(hWindow, GWLP_USERDATA));
		if (!window)
			return DefWindowProc(hWindow, uMessage, wParam, lParam);

		Win32Window::Titlebar::HoveredButton title_bar_hovered_button = window->m_Titlebar.CurrentHoveredButton;

		// Map button clicks to the right uMessages for the window
		if (title_bar_hovered_button == Win32Window::Titlebar::HoveredButton::Close) 
		{
			PostMessageW(hWindow, WM_CLOSE, 0, 0);
			return 0;
		}
		else if (title_bar_hovered_button == Win32Window::Titlebar::HoveredButton::Minimize) 
		{
			ShowWindow(hWindow, SW_MINIMIZE);
			return 0;
		}
		else if (title_bar_hovered_button == Win32Window::Titlebar::HoveredButton::Maximize) 
		{			
			int mode = Win32IsWindowMaximized(hWindow) ? SW_NORMAL : SW_MAXIMIZE;
			ShowWindow(hWindow, mode);
			return 0;
		}

		return DefWindowProc(hWindow, uMessage, wParam, lParam);
	}

	static LRESULT Win32HandleWM_SETCURSOR(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
	{
		auto window = (const Win32Window*)(GetWindowLongPtr(hWindow, GWLP_USERDATA));
		if (!window)
			return DefWindowProc(hWindow, uMessage, wParam, lParam);

		switch(LOWORD(lParam))
		{
		case HTCLIENT:
		{
			SetCursor(window->m_Style.Cursor.LeftPointer);
		} break;
		case HTRIGHT:
		case HTLEFT:
		{
			SetCursor(window->m_Style.Cursor.ResizeHorizontal);
		} break;
			
		case HTTOP:
		case HTBOTTOM:
		{
			SetCursor(window->m_Style.Cursor.ResizeVertical);
		} break;
		case HTTOPRIGHT:
		case HTBOTTOMLEFT:
		{
			SetCursor(window->m_Style.Cursor.ResizeBDiag);
		} break;
		case HTBOTTOMRIGHT:
		case HTTOPLEFT:
		{
			SetCursor(window->m_Style.Cursor.ResizeFDiag);
		} break;
		default:
			SetCursor(window->m_Style.Cursor.LeftPointer);
		}

		return TRUE;
	}

	static LRESULT CALLBACK WindowProc(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
	{
		switch (uMessage) 
		{
		case WM_NCCALCSIZE: 
		{
			return Win32HandleWM_NCCALCSIZE(hWindow, uMessage, wParam, lParam);
		} break;
		case WM_CREATE: 
		{
			return Win32HandleWM_CREATE(hWindow, uMessage, wParam, lParam);
		} break;
		case WM_ACTIVATE: 
		{
			return Win32HandleWM_ACTIVATE(hWindow, uMessage, wParam, lParam);
		} break;
		case WM_NCHITTEST: 
		{
			return Win32HandleWM_NCHITTEST(hWindow, uMessage, wParam, lParam);
		} break;
		case WM_PAINT: 
		{
			return Win32HandleWM_PAINT(hWindow, uMessage, wParam, lParam);
		} break;
		case WM_SIZE:
		{
			return Win32HandleWM_SIZE(hWindow, uMessage, wParam, lParam);
		} break;
		case WM_NCMOUSEMOVE:
		{
			return Win32HandleWM_NCMOUSEMOVE(hWindow, uMessage, wParam, lParam);
		} break;
		case WM_MOUSEMOVE: 
		{
			return Win32HandleWM_MOUSEMOVE(hWindow, uMessage, wParam, lParam);
		} break;
		case WM_NCLBUTTONDOWN: 
		{
			return Win32HandleWM_NCLBUTTONDOWN(hWindow, uMessage, wParam, lParam);
		} break;
		case WM_NCLBUTTONUP: 
		{
			return Win32HandleWM_NCLBUTTONUP(hWindow, uMessage, wParam, lParam);
		} break;
		case WM_SETCURSOR: 
		{
			return Win32HandleWM_SETCURSOR(hWindow, uMessage, wParam, lParam);
		} break;
		case WM_CLOSE:
		{
			auto window = (Window*)(GetWindowLongPtr(hWindow, GWLP_USERDATA));
			if (window)
				window->Close();
		} break;
		case WM_DESTROY: 
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
		{
			return DefWindowProc(hWindow, uMessage, wParam, lParam);
		} break;
		}

		return DefWindowProc(hWindow, uMessage, wParam, lParam);
	}

	static LRESULT CALLBACK OpenGLPanelProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		auto window = (Win32Window*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if (!window)
			return DefWindowProc(hWnd, uMsg, wParam, lParam);

		switch (uMsg)
		{
		case WM_MOUSEMOVE:
		{
			window->m_Titlebar.CurrentHoveredButton = Win32Window::Titlebar::HoveredButton::None;
			InvalidateRect(window->m_Win32Data.WindowHandle, &Win32GetTitlebarRect(window->m_Win32Data.WindowHandle), FALSE);
		} break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}