#include "Win32Window.h"

#include <uxtheme.h>
#include <vssym32.h>
#include <windowsx.h>

#include "Log.h"

#define UNUSED(name) (void)name

namespace mwl
{
	constexpr auto GL_FALSE = 0;
	constexpr auto GL_TRUE = 1;

	constexpr auto WGL_DRAW_TO_WINDOW_ARB = 0x2001;
	constexpr auto WGL_ACCELERATION_ARB = 0x2003;
	constexpr auto WGL_SUPPORT_OPENGL_ARB = 0x2010;
	constexpr auto WGL_DOUBLE_BUFFER_ARB = 0x2011;
	constexpr auto WGL_PIXEL_TYPE_ARB = 0x2013;
	constexpr auto WGL_COLOR_BITS_ARB = 0x2014;
	constexpr auto WGL_DEPTH_BITS_ARB = 0x2022;
	constexpr auto WGL_STENCIL_BITS_ARB = 0x2023;
	constexpr auto WGL_FULL_ACCELERATION_ARB = 0x2027;
	constexpr auto WGL_TYPE_RGBA_ARB = 0x202B;
	constexpr auto WGL_CONTEXT_MAJOR_VERSION_ARB = 0x2091;
	constexpr auto WGL_CONTEXT_MINOR_VERSION_ARB = 0x2092;
	constexpr auto WGL_CONTEXT_PROFILE_MASK_ARB = 0x9126;
	constexpr auto WGL_CONTEXT_CORE_PROFILE_BIT_ARB = 0x00000001;

	typedef BOOL(WINAPI* PFNWGLCHOOSEPIXELFOTMATARBPROC)(HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
	PFNWGLCHOOSEPIXELFOTMATARBPROC wglChoosePixelFormatARB;

	typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hdc, HGLRC hShareContext, const int* attribList);
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

	typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC)(int interval);
	PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

	static int GetMajorOpenGLVersion(OpenGLVersion openglVersion)
	{
		switch (openglVersion)
		{
			case mwl::OpenGLVersion::OPENGL_1_0: return 1;
			case mwl::OpenGLVersion::OPENGL_1_1: return 1;
			case mwl::OpenGLVersion::OPENGL_1_2: return 1;
			case mwl::OpenGLVersion::OPENGL_1_3: return 1;
			case mwl::OpenGLVersion::OPENGL_1_4: return 1;
			case mwl::OpenGLVersion::OPENGL_1_5: return 1;
			case mwl::OpenGLVersion::OPENGL_2_0: return 2;
			case mwl::OpenGLVersion::OPENGL_2_1: return 2;
			case mwl::OpenGLVersion::OPENGL_3_0: return 3;
			case mwl::OpenGLVersion::OPENGL_3_1: return 3;
			case mwl::OpenGLVersion::OPENGL_3_2: return 3;
			case mwl::OpenGLVersion::OPENGL_3_3: return 3;
			case mwl::OpenGLVersion::OPENGL_4_0: return 4;
			case mwl::OpenGLVersion::OPENGL_4_1: return 4;
			case mwl::OpenGLVersion::OPENGL_4_2: return 4;
			case mwl::OpenGLVersion::OPENGL_4_3: return 4;
			case mwl::OpenGLVersion::OPENGL_4_4: return 4;
			case mwl::OpenGLVersion::OPENGL_4_5: return 4;
			case mwl::OpenGLVersion::OPENGL_4_6: return 4;
		}

		return 3;
	}

	static int GetMinorOpenGLVersion(OpenGLVersion openglVersion)
	{
		switch (openglVersion)
		{
			case mwl::OpenGLVersion::OPENGL_1_0: return 0;
			case mwl::OpenGLVersion::OPENGL_1_1: return 1;
			case mwl::OpenGLVersion::OPENGL_1_2: return 2;
			case mwl::OpenGLVersion::OPENGL_1_3: return 3;
			case mwl::OpenGLVersion::OPENGL_1_4: return 4;
			case mwl::OpenGLVersion::OPENGL_1_5: return 5;
			case mwl::OpenGLVersion::OPENGL_2_0: return 0;
			case mwl::OpenGLVersion::OPENGL_2_1: return 1;
			case mwl::OpenGLVersion::OPENGL_3_0: return 0;
			case mwl::OpenGLVersion::OPENGL_3_1: return 1;
			case mwl::OpenGLVersion::OPENGL_3_2: return 2;
			case mwl::OpenGLVersion::OPENGL_3_3: return 3;
			case mwl::OpenGLVersion::OPENGL_4_0: return 0;
			case mwl::OpenGLVersion::OPENGL_4_1: return 1;
			case mwl::OpenGLVersion::OPENGL_4_2: return 2;
			case mwl::OpenGLVersion::OPENGL_4_3: return 3;
			case mwl::OpenGLVersion::OPENGL_4_4: return 4;
			case mwl::OpenGLVersion::OPENGL_4_5: return 5;
			case mwl::OpenGLVersion::OPENGL_4_6: return 6;
		}

		return 3;
	}

	Window* Create(const WindowProps& windowProps /*= WindowProps()*/)
	{
		return new Win32Window(windowProps);
	}

	Win32Window::Win32Window(const WindowProps& windowProps)
	{
		m_State.Title					= windowProps.Title;
		m_State.Width					= windowProps.Width;
		m_State.Height					= windowProps.Height;
		m_State.VSync					= windowProps.VSync;
		m_State.Closed					= windowProps.Closed;
		m_State.OpenGLContextVersion	= windowProps.OpenGLContextVersion;
		m_State.MouseX					= 0;
		m_State.MouseY					= 0;
		m_State.MouseClicked			= MouseButton::None;
		m_State.FullscreenRecoverPlacement = { 0 };
		m_State.IsFullscreen			= false;

		if (!CreateWin32Window())
		{
			LOG_CRITICAL("Could not create window");
			Close();
		}
		else
			LOG_DEBUG("Window successfully created");

		if (!CreateOpenGLContext(m_State.OpenGLContextVersion))
		{
			LOG_CRITICAL("Could not create OpenGL context");
			Close();
		}
		else
			LOG_DEBUG("OpenGL context successfully created");

		ShowWindow(m_Win32Data.WindowHandle, SW_SHOW);
		EnableVSync(m_State.VSync);

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
	}

	void Win32Window::SwapBuffers()
	{
		::SwapBuffers(GetDC(m_Win32Data.OpenGLHandle));
	}

	void Win32Window::Close()
	{
		m_State.Closed = true;
	}

	void Win32Window::EnableVSync(bool enable)
	{
		m_State.VSync = enable;

		int interval = enable ? 1 : 0;
		wglSwapIntervalEXT(interval);
	}

	void Win32Window::MakeContextCurrent()
	{
		wglMakeCurrent(GetDC(m_Win32Data.OpenGLHandle), m_Win32Data.OpenGLRenderingContext);
	}

	bool Win32Window::Closed() const
	{
		return m_State.Closed;
	}

	bool Win32Window::IsFullscreen() const
	{
		return m_State.IsFullscreen;
	}

	uint32_t Win32Window::GetWidth() const
	{
		return m_State.Width;
	}

	uint32_t Win32Window::GetHeight() const
	{
		return m_State.Height;
	}

	void* Win32Window::GetNative()
	{
		return (void*)m_Win32Data.OpenGLHandle;
	}

	void Win32Window::SetCursor(const Cursor& cursor)
	{
		Window::SetCursor(cursor);

		m_Style.Cursor.LeftPointer			= LoadCursorFromFile(cursor.Pointer.value_or(L"").c_str());
		m_Style.Cursor.HandPointer			= LoadCursorFromFile(cursor.Hand.value_or(L"").c_str());
		m_Style.Cursor.Text					= LoadCursorFromFile(cursor.Text.value_or(L"").c_str());
		m_Style.Cursor.ResizeHorizontal		= LoadCursorFromFile(cursor.Horz.value_or(L"").c_str());
		m_Style.Cursor.ResizeVertical		= LoadCursorFromFile(cursor.Vert.value_or(L"").c_str());
		m_Style.Cursor.ResizeFDiag			= LoadCursorFromFile(cursor.Dgn1.value_or(L"").c_str());
		m_Style.Cursor.ResizeBDiag			= LoadCursorFromFile(cursor.Dgn2.value_or(L"").c_str());
	}

	void Win32Window::SetFullscreen(bool fullscreen)
	{
		m_State.IsFullscreen = fullscreen;

		if (fullscreen)
		{
			LONG style = GetWindowLong(m_Win32Data.WindowHandle, GWL_STYLE);
			style &= ~WS_MAXIMIZEBOX;
			SetWindowLong(m_Win32Data.WindowHandle, GWL_STYLE, style);
			
			WINDOWPLACEMENT wp = { sizeof(WINDOWPLACEMENT) };
			GetWindowPlacement(m_Win32Data.WindowHandle, &wp);
			if (wp.showCmd == SW_MAXIMIZE)
				ShowWindow(m_Win32Data.WindowHandle, SW_RESTORE);
			
			ShowWindow(m_Win32Data.WindowHandle, SW_MAXIMIZE);
		}
		else
		{
			LONG style = GetWindowLong(m_Win32Data.WindowHandle, GWL_STYLE);
			style |= WS_MAXIMIZEBOX;
			SetWindowLong(m_Win32Data.WindowHandle, GWL_STYLE, style);
			ShowWindow(m_Win32Data.WindowHandle, SW_RESTORE);
		}
	}

	bool Win32Window::CreateWin32Window()
	{
		WNDCLASSEX wc		= { 0 };
		wc.cbSize			= sizeof(wc);
		wc.style			= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc		= WindowProc;
		wc.hInstance		= GetModuleHandle(nullptr);
		wc.hCursor			= LoadCursor(nullptr, IDC_ARROW);
		wc.lpszClassName	= m_State.Title.c_str();

		if (!RegisterClassEx(&wc))
		{
			LOG_ERROR("Could not register '{0}' window class", m_State.Title);
			return false;
		}
		else 
			LOG_DEBUG("Window class '{0}' registered", m_State.Title);

		DWORD style = WS_CLIPCHILDREN | WS_THICKFRAME | WS_MAXIMIZEBOX;
		DWORD exStyle = WS_EX_LEFT;

		RECT desired_window_rect = { 0, 0, m_State.Width, m_State.Height };
		AdjustWindowRectEx(&desired_window_rect, style, false, exStyle);
		desired_window_rect.bottom += m_Titlebar.Height;

		int width = desired_window_rect.right - desired_window_rect.left;
		int height = desired_window_rect.bottom - desired_window_rect.top;

		m_Win32Data.WindowHandle = CreateWindowEx(exStyle,
			m_State.Title.c_str(),
			m_State.Title.c_str(),
			style,
			CW_USEDEFAULT, CW_USEDEFAULT,
			width, height,
			nullptr,
			nullptr,
			GetModuleHandle(nullptr),
			nullptr);

		RECT clientRect = { 0 };
		GetClientRect(m_Win32Data.WindowHandle, &clientRect);
		clientRect.bottom -= m_Titlebar.Height;
		m_State.Width = clientRect.right - clientRect.left;
		m_State.Height = clientRect.bottom - clientRect.top;

		return m_Win32Data.WindowHandle != nullptr;
	}

	bool Win32Window::CreateOpenGLContext(OpenGLVersion openglVerson)
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

		m_Win32Data.OpenGLRenderingContext = CreateOpenGLRenderingContext(openglVerson);

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

		if (!wglMakeCurrent(dummyDC,  dummyContext))
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
			LOG_DEBUG("Dummy OpenGL context successfully created: '{0}'", wc.lpszClassName);

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
			LOG_DEBUG("Dummy OpenGL window successfully created");

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
		wc.lpszClassName = opengl_panel_name.c_str();

		if (!RegisterClassEx(&wc))
		{
			LOG_ERROR("Could not register '{0}' class", opengl_panel_name.c_str());
			return nullptr;
		}
		else
			LOG_DEBUG("Window class '{0}' registered", opengl_panel_name.c_str());

		HWND opengl_panel_handle = CreateWindowEx(WS_EX_LEFT,
			opengl_panel_name.c_str(),
			opengl_panel_name.c_str(),
			WS_CHILD | WS_VISIBLE | CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
			0,
			m_Titlebar.Height,
			m_State.Width,
			m_State.Height,
			m_Win32Data.WindowHandle,
			nullptr,
			GetModuleHandle(nullptr),
			nullptr);

		RECT rect;
		GetClientRect(m_Win32Data.WindowHandle, &rect);
		rect.top += m_Titlebar.Height;

		// Set the position and size of the child window to cover the entire parent window
		//MoveWindow(opengl_panel_handle, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);

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

	std::vector<int> Win32Window::GetOpenGLContextAttribs(OpenGLVersion openglVerson) const
	{
		std::vector<int> glAttribs = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, GetMajorOpenGLVersion(openglVerson),
			WGL_CONTEXT_MINOR_VERSION_ARB, GetMinorOpenGLVersion(openglVerson),
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

	HGLRC Win32Window::CreateOpenGLRenderingContext(OpenGLVersion openglVerson)
	{
		std::vector<int> gl_attribs = GetOpenGLContextAttribs(openglVerson);

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

	static void SpawnWindowResizeEvent(const Win32Window* window)
	{
		if (!window)
			return;

		if (!window->Callbacks.WindowResizeCallback)
			return;

		WindowResizeEvent e;
		e.Width	 = window->m_State.Width;
		e.Height = window->m_State.Height;

		window->Callbacks.WindowResizeCallback(e);
	}

	static void SpawnMouseMoveEvent(const Win32Window* window)
	{
		if (!window)
			return;

		if (!window->Callbacks.MouseMovedCallback)
			return;

		MouseMovedEvent e;
		e.X = window->m_State.MouseX;
		e.Y = window->m_State.MouseY;

		window->Callbacks.MouseMovedCallback(e);
	}

	static void SpawnMouseButtonDownEvent(const Win32Window* window, MouseButton button)
	{
		if (!window)
			return;

		if (!window->Callbacks.MouseButtonDownCallback)
			return;

		MouseButtonDownEvent e;
		e.X = window->m_State.MouseX;
		e.Y = window->m_State.MouseY;
		e.Button = button;

		window->Callbacks.MouseButtonDownCallback(e);
	}

	static void SpawnMouseButtonUpEvent(const Win32Window* window, MouseButton button)
	{
		if (!window)
			return;

		if (!window->Callbacks.MouseButtonUpCallback)
			return;

		MouseButtonUpEvent e;
		e.X = window->m_State.MouseX;
		e.Y = window->m_State.MouseY;
		e.Button = button;

		window->Callbacks.MouseButtonUpCallback(e);
	}

	static void SpawnMouseButtonClickedEvent(const Win32Window* window, MouseButton button)
	{
		if (!window)
			return;

		if (!window->Callbacks.MouseClickedCallback)
			return;

		MouseClickedEvent e;
		e.X = window->m_State.MouseX;
		e.Y = window->m_State.MouseY;
		e.Button = button;

		window->Callbacks.MouseClickedCallback(e);
	}

	void SpawnMouseScrollEvent(const Win32Window* window, int delta)
	{
		if (!window)
			return;

		if (!window->Callbacks.MouseScrollCallback)
			return;

		MouseScrollEvent e;
		e.Delta = delta;

		window->Callbacks.MouseScrollCallback(e);
	}

	static void SpawnKeyDownEvent(const Win32Window* window, KeyCode keyCode, bool repeat)
	{
		if (!window)
			return;

		if (!window->Callbacks.KeyDownCallback)
			return;

		KeyDownEvent e;
		e.Key = keyCode;
		e.Repeat = repeat;

		if(e.Key != KeyCode::None)
			window->Callbacks.KeyDownCallback(e);
	}

	static void SpawnKeyUpEvent(const Win32Window* window, KeyCode keyCode)
	{
		if (!window)
			return;

		if (!window->Callbacks.KeyUpCallback)
			return;

		KeyUpEvent e;
		e.Key = keyCode;

		if (e.Key != KeyCode::None)
			window->Callbacks.KeyUpCallback(e);
	}

	static KeyCode Win32KeyToKeyCode(WPARAM keyCode)
	{
		switch (keyCode)
		{
		case '0': return KeyCode::D0;
		case '1': return KeyCode::D1;
		case '2': return KeyCode::D2;
		case '3': return KeyCode::D3;
		case '4': return KeyCode::D4;
		case '5': return KeyCode::D5;
		case '6': return KeyCode::D6;
		case '7': return KeyCode::D7;
		case '8': return KeyCode::D8;
		case '9': return KeyCode::D9;

		case 'A': return KeyCode::A;
		case 'B': return KeyCode::B;
		case 'C': return KeyCode::C;
		case 'D': return KeyCode::D;
		case 'E': return KeyCode::E;
		case 'F': return KeyCode::F;
		case 'G': return KeyCode::G;
		case 'H': return KeyCode::H;
		case 'I': return KeyCode::I;
		case 'J': return KeyCode::J;
		case 'K': return KeyCode::K;
		case 'L': return KeyCode::L;
		case 'M': return KeyCode::M;
		case 'N': return KeyCode::N;
		case 'O': return KeyCode::O;
		case 'P': return KeyCode::P;
		case 'Q': return KeyCode::Q;
		case 'R': return KeyCode::R;
		case 'S': return KeyCode::S;
		case 'T': return KeyCode::T;
		case 'U': return KeyCode::U;
		case 'V': return KeyCode::V;
		case 'W': return KeyCode::W;
		case 'X': return KeyCode::X;
		case 'Y': return KeyCode::Y;
		case 'Z': return KeyCode::Z;

		case 0x70: return KeyCode::F1;
		case 0x71: return KeyCode::F2;
		case 0x72: return KeyCode::F3;
		case 0x73: return KeyCode::F4;
		case 0x74: return KeyCode::F5;
		case 0x75: return KeyCode::F6;
		case 0x76: return KeyCode::F7;
		case 0x77: return KeyCode::F8;
		case 0x78: return KeyCode::F9;
		case 0x79: return KeyCode::F10;
		case 0x7A: return KeyCode::F11;
		case 0x7B: return KeyCode::F12;
		case 0x7C: return KeyCode::F13;
		case 0x7D: return KeyCode::F14;
		case 0x7E: return KeyCode::F15;
		case 0x7F: return KeyCode::F16;
		case 0x80: return KeyCode::F17;
		case 0x81: return KeyCode::F18;
		case 0x82: return KeyCode::F19;
		case 0x83: return KeyCode::F20;
		case 0x84: return KeyCode::F21;
		case 0x85: return KeyCode::F22;
		case 0x86: return KeyCode::F23;
		case 0x87: return KeyCode::F24;

		case VK_ESCAPE:		return KeyCode::Escape;
		case VK_RETURN:		return KeyCode::Enter;
		case VK_SPACE:		return KeyCode::Space;
		case VK_LCONTROL:	return KeyCode::LeftControl;
		case VK_BACK:		return KeyCode::Backspace;

		default: return KeyCode::None;
		}

		return KeyCode::None;
	}

	static int Win32DpiScale(int value, UINT dpi)
	{
		return (int)((float)value * (float)dpi / 96.0f);
	}

	static Win32Window::Titlebar::ButtonRects Win32GetTitlebarButtonRects(HWND handle, const RECT* title_bar_rect)
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
		return Win32HandleWM_CREATE(hWindow, uMessage, wParam, lParam);
	}

	static LRESULT Win32HandleWM_NCHITTEST(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
	{
		auto window = (Win32Window*)GetWindowLongPtr(hWindow, GWLP_USERDATA);
		if (!window)
			return DefWindowProc(hWindow, uMessage, wParam, lParam);

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
		if (cursor_point.y > 0 && cursor_point.y < frame_y + padding && !Win32IsWindowMaximized(hWindow))
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

		HBRUSH title_bar_brush = CreateSolidBrush(style.Colors.Titlebar);
		HBRUSH title_bar_hover_brush = CreateSolidBrush(style.Colors.TitlebarButtonHover);

		RECT title_bar_rect = Win32GetTitlebarRect(hWindow);

		// Title Bar Background
		FillRect(hdc, &title_bar_rect, title_bar_brush);

		COLORREF title_bar_item_color = has_focus ? style.Colors.TitlebarItem : style.Colors.TitlebarItemNoFocus;

		HBRUSH button_icon_brush = CreateSolidBrush(title_bar_item_color);
		HPEN button_icon_pen = CreatePen(PS_SOLID, 1, title_bar_item_color);

		Win32Window::Titlebar::ButtonRects button_rects = Win32GetTitlebarButtonRects(hWindow, &title_bar_rect);

		UINT dpi = GetDpiForWindow(hWindow);
		int icon_dimension = Win32DpiScale(8, dpi);
		
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
				HBRUSH fill_brush = CreateSolidBrush(RGB(0xAA, 0, 0));
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
		//LOGFONT logical_font;
		//HFONT old_font = NULL;
		//if (SUCCEEDED(SystemParametersInfoForDpi(SPI_GETICONTITLELOGFONT, sizeof(logical_font), &logical_font, false, dpi))) {
		//	HFONT theme_font = CreateFontIndirect(&logical_font);
		//	old_font = (HFONT)SelectObject(hdc, theme_font);
		//}

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
			window->m_State.Title.c_str(),
			-1,
			DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS,
			&title_bar_text_rect,
			&draw_theme_options
		);

		//if (old_font) SelectObject(hdc, old_font);
		CloseThemeData(theme);

		EndPaint(hWindow, &ps);

		return 0;
	}

	static LRESULT Win32HandleWM_SIZE(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
	{
		auto window = (Win32Window*)GetWindowLongPtr(hWindow, GWLP_USERDATA);
		if (!window)
			return DefWindowProc(hWindow, uMessage, wParam, lParam);

		auto titlebar_height = window->IsFullscreen() ? 0 : window->m_Titlebar.Height;
		int width = LOWORD(lParam);
		int height = HIWORD(lParam) - titlebar_height;

		window->m_State.Width = width;
		window->m_State.Height = height;

		MoveWindow(window->m_Win32Data.OpenGLHandle, 0, titlebar_height, width, height, TRUE);

		return 0;
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
		Win32Window::Titlebar::ButtonRects button_rects = Win32GetTitlebarButtonRects(hWindow, &title_bar_rect);

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

		HCURSOR leftPointer = window->m_Style.Cursor.LeftPointer ? window->m_Style.Cursor.LeftPointer : LoadCursor(nullptr, IDC_ARROW);
		HCURSOR resizeHorizontal = window->m_Style.Cursor.ResizeHorizontal ? window->m_Style.Cursor.ResizeHorizontal : LoadCursor(nullptr, IDC_SIZEWE);
		HCURSOR resizeVertical = window->m_Style.Cursor.ResizeVertical ? window->m_Style.Cursor.ResizeVertical : LoadCursor(nullptr, IDC_SIZENS);
		HCURSOR resizeBDiag = window->m_Style.Cursor.ResizeBDiag ? window->m_Style.Cursor.ResizeBDiag : LoadCursor(nullptr, IDC_SIZENESW);
		HCURSOR resizeFDiag = window->m_Style.Cursor.ResizeFDiag ? window->m_Style.Cursor.ResizeFDiag: LoadCursor(nullptr, IDC_SIZENWSE);

		switch(LOWORD(lParam))
		{
		case HTCLIENT:
		{
			SetCursor(leftPointer);
		} break;
		case HTRIGHT:
		case HTLEFT:
		{
			SetCursor(resizeHorizontal);
		} break;
			
		case HTTOP:
		case HTBOTTOM:
		{
			SetCursor(resizeVertical);
		} break;
		case HTTOPRIGHT:
		case HTBOTTOMLEFT:
		{
			SetCursor(resizeBDiag);
		} break;
		case HTBOTTOMRIGHT:
		case HTTOPLEFT:
		{
			SetCursor(resizeFDiag);
		} break;
		default:
			SetCursor(leftPointer);
		}

		return TRUE;
	}
	
	static LRESULT CALLBACK WindowProc(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
	{
		auto window = (const Win32Window*)(GetWindowLongPtr(hWindow, GWLP_USERDATA));

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
			auto res = Win32HandleWM_SIZE(hWindow, uMessage, wParam, lParam);
			SpawnWindowResizeEvent(window);
			return res;
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
		case WM_KEYDOWN:
		{
			KeyCode keyCode = Win32KeyToKeyCode(wParam);
			uint32_t repeat = lParam & (1 << 30);

			SpawnKeyDownEvent(window, keyCode, repeat);
		} break;
		case WM_KEYUP:
		{
			KeyCode keyCode = Win32KeyToKeyCode(wParam);

			SpawnKeyUpEvent(window, keyCode);
		} break;
		default:
		{
			return DefWindowProc(hWindow, uMessage, wParam, lParam);
		} break;
		}

		return DefWindowProc(hWindow, uMessage, wParam, lParam);
	}

	static bool UpdateWindowMousePosition(Win32Window* window, LPARAM lParam)
	{
		if (!window)
			return false;

		auto mouse_x = GET_X_LPARAM(lParam);
		auto mouse_y = GET_Y_LPARAM(lParam);

		if (window->m_State.MouseX != mouse_x || window->m_State.MouseY != mouse_y)
		{
			window->m_State.MouseX = mouse_x;
			window->m_State.MouseY = mouse_y;

			return true;
		}

		return false;
	}

	static LRESULT CALLBACK OpenGLPanelProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		auto window = (Win32Window*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		if (!window)
			return DefWindowProc(hWnd, uMsg, wParam, lParam);

		//auto logIt = xmsglist.find(uMsg);
		//if (logIt != xmsglist.end())
		//	LOG_WARNING("OpenGL panel {0}", logIt->second);

		switch (uMsg)
		{
			case WM_MOUSEMOVE:
			{
				if (window->m_Titlebar.CurrentHoveredButton != Win32Window::Titlebar::HoveredButton::None)
				{
					window->m_Titlebar.CurrentHoveredButton = Win32Window::Titlebar::HoveredButton::None;

					RECT titlebar_rect = Win32GetTitlebarRect(window->m_Win32Data.WindowHandle);
					Win32Window::Titlebar::ButtonRects titlebar_button_rects = Win32GetTitlebarButtonRects(window->m_Win32Data.WindowHandle, &titlebar_rect);
					RECT titlebuttons_rect = { 0 };
					titlebuttons_rect = titlebar_button_rects.Minimize;
					titlebuttons_rect.right = titlebar_button_rects.Close.right;

					InvalidateRect(window->m_Win32Data.WindowHandle, &titlebuttons_rect, FALSE);
				}
			
				if (UpdateWindowMousePosition(window, lParam))
				{
					window->m_State.MouseClicked = MouseButton::None;

					SpawnMouseMoveEvent(window);
				}

				return 0;
			} break;
			case WM_LBUTTONDOWN:
			{
				window->m_State.MouseClicked = MouseButton::Left;

				UpdateWindowMousePosition(window, lParam);
				SpawnMouseButtonDownEvent(window, MouseButton::Left);

				return 0;
			} break;
			case WM_LBUTTONUP:
			{
				UpdateWindowMousePosition(window, lParam);
				SpawnMouseButtonUpEvent(window, MouseButton::Left);

				if (window->m_State.MouseClicked == MouseButton::Left)
					SpawnMouseButtonClickedEvent(window, MouseButton::Left);

				return 0;
			} break;
			case WM_RBUTTONDOWN:
			{
				window->m_State.MouseClicked = MouseButton::Right;

				UpdateWindowMousePosition(window, lParam);
				SpawnMouseButtonDownEvent(window, MouseButton::Right);

				return 0;
			} break;
			case WM_RBUTTONUP:
			{
				UpdateWindowMousePosition(window, lParam);
				SpawnMouseButtonUpEvent(window, MouseButton::Right);

				if (window->m_State.MouseClicked == MouseButton::Right)
					SpawnMouseButtonClickedEvent(window, MouseButton::Right);

				return 0;
			} break;
			case WM_MBUTTONDOWN:
			{
				window->m_State.MouseClicked = MouseButton::Middle;

				UpdateWindowMousePosition(window, lParam);
				SpawnMouseButtonDownEvent(window, MouseButton::Middle);

				return 0;
			} break;
			case WM_MBUTTONUP:
			{
				UpdateWindowMousePosition(window, lParam);
				SpawnMouseButtonUpEvent(window, MouseButton::Middle);

				if (window->m_State.MouseClicked == MouseButton::Middle)
					SpawnMouseButtonClickedEvent(window, MouseButton::Middle);

				return 0;
			} break;
			case WM_MOUSEWHEEL:
			{
				int delta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
				SpawnMouseScrollEvent(window, delta);

				return 0;
			} break;
			case WM_SETCURSOR:
			{
				return Win32HandleWM_SETCURSOR(hWnd, uMsg, wParam, lParam);
			} break;
			default:
			{
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}