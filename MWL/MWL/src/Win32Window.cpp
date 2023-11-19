#include "Win32Window.h"

namespace mwl
{
	Window* Create(const WindowProps& windowProps /*= WindowProps()*/)
	{
		return new Win32Window(windowProps);
	}

	Win32Window::Win32Window(const WindowProps& windowProps)
	{
		m_WindowData.Closed = false;
		m_WindowData.Width = windowProps.Width;
		m_WindowData.Height = windowProps.Height;
		m_WindowData.Title = windowProps.Title;
	}

	Win32Window::~Win32Window()
	{

	}

	void Win32Window::Update()
	{

	}

	void Win32Window::Close()
	{

	}

	void Win32Window::EnableVSync()
	{

	}

	bool Win32Window::Closed() const
	{
		return false;
	}

	uint32_t Win32Window::GetWidth() const
	{
		return 0;
	}

	uint32_t Win32Window::GetHeight() const
	{
		return 0;
	}

	bool Win32Window::CreateWin32Window()
	{
		WNDCLASSEX wc = { };

		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.lpszClassName = m_WindowData.Title.c_str();

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
			NULL,
			NULL,
			GetModuleHandle(NULL),
			NULL);

		RECT clientRect;
		GetClientRect(m_Win32Data.WindowHandle, &clientRect);
		m_WindowData.Width = clientRect.right - clientRect.left;
		m_WindowData.Height = clientRect.bottom - clientRect.top;

		return m_Win32Data.WindowHandle != NULL;
	}

	bool Win32Window::CreateOpenGLContext(int major, int minor)
	{
		if (!InitOpenGLContext())
		{
			LOG_ERROR("Failed to init dummy OpenGL context");
			return false;
		}

		int pixelFormatAttribs[] = {
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

		HDC dc = GetDC(m_WindowHandle);

		int pixel_format;
		UINT num_formats;
		wglChoosePixelFormatARB(dc, pixelFormatAttribs, 0, 1, &pixel_format, &num_formats);
		if (!num_formats)
			DIE("Failed to set the OpenGL pixel format.");

		PIXELFORMATDESCRIPTOR pfd;
		DescribePixelFormat(dc, pixel_format, sizeof(pfd), &pfd);
		if (!SetPixelFormat(dc, pixel_format, &pfd))
			DIE("Failed to set the OpenGL pixel format.");

		int glAttribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, major,
			WGL_CONTEXT_MINOR_VERSION_ARB, minor,
			WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0,
		};

		HGLRC glContext = wglCreateContextAttribsARB(dc, 0, glAttribs);
		if (!glContext)
			DIE("Failed to create OpenGL context.");

		if (!wglMakeCurrent(dc, glContext))
			DIE("Failed to activate OpenGL rendering context.");

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
			wglMakeCurrent(dummyDC, 0);
			wglDeleteContext(dummyContext);
			ReleaseDC(dummyWindow, dummyDC);
			DestroyWindow(dummyWindow);
			return false;
		}

		if (!wglMakeCurrent(dummyDC, dummyContext))
		{
			LOG_ERROR("Failed to activate dummy OpenGL rendering context.");
			wglMakeCurrent(dummyDC, 0);
			wglDeleteContext(dummyContext);
			ReleaseDC(dummyWindow, dummyDC);
			DestroyWindow(dummyWindow);
			return false;
		}

		LoadOpenGLExtensions();

		wglMakeCurrent(dummyDC, 0);
		wglDeleteContext(dummyContext);
		ReleaseDC(dummyWindow, dummyDC);
		DestroyWindow(dummyWindow);

		return true;
	}

	HWND Win32Window::CreateDummyWindow()
	{
		WNDCLASSA wc = { };

		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = DefWindowProcA;
		wc.hInstance = GetModuleHandle(0);
		wc.lpszClassName = "MONK_WGL_DUMMY_WINDOW";

		if (!RegisterClassA(&wc))
			LOG_ERROR("Failed to register dummy OpenGL window");
		else
			LOG_INFO("Dummy OpenGL context successfully created");

		HWND window = CreateWindowExA(
			0,
			wc.lpszClassName,
			"Dummy OpenGL Window",
			0,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			0,
			0,
			wc.hInstance,
			0);

		if (!window)
			LOG_ERROR("Failed to create dummy OpenGL window");
		else
			LOG_INFO("Dummy OpenGL window successfully created");

		HDC dc = GetDC(window);

		PIXELFORMATDESCRIPTOR pfd = { 0 };
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.cColorBits = 32;
		pfd.cAlphaBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;

		int pixelFormat = ChoosePixelFormat(dc, &pfd);

		if (!pixelFormat)
			LOG_ERROR("Failed to find a suitable pixel format.");

		if (!SetPixelFormat(dc, pixelFormat, &pfd))
			LOG_ERROR("Failed to set the pixel format.");

		return window;
	}

	void Win32Window::LoadOpenGLExtensions()
	{
		wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)utils::OpenGLLoader::LoadFunction("wglCreateContextAttribsARB");
		wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFOTMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");	
	}

	LRESULT CALLBACK WindowProc(HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hWindow, uMessage, wParam, lParam);
	}
}