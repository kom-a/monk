#include "Window.h"

#include "core/Log.h"
#include "utils/OpenGL.h"
#include "events/WindowEvent.h"
#include "events/KeyEvent.h"
#include "events/MouseEvent.h"

#include "input/Input.h"

#include <string>

namespace monk
{
	std::unordered_map<HWND, Window*> Window::m_WindowTable;

	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Window* window = Window::GetWindowByHandle(hWnd);
	
		if (!window || !window->m_WindowData.EventCallbackFn)
			return DefWindowProc(hWnd, uMsg, wParam, lParam);

		switch (uMsg)
		{
		case WM_CLOSE:
		{
			WindowCloseEvent e;
			window->m_WindowData.EventCallbackFn(e);

			window->Close();
		}break;
		case WM_SIZE:
		{
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);

			window->m_WindowData.Width = width;
			window->m_WindowData.Height = height;

			WindowResizeEvent e(width, height);
			window->m_WindowData.EventCallbackFn(e);
		} break;
		case WM_KEYDOWN:
		{
			KeyCode key = (KeyCode) wParam;
			bool isRepeat = lParam & (1 << 30);

			if (!isRepeat)
				Input::s_KeyDown[key] = true;

			KeyDownEvent e(key, isRepeat);
			window->m_WindowData.EventCallbackFn(e);
		} break;
		case WM_KEYUP:
		{
			KeyCode key = (KeyCode)wParam;
			Input::s_KeyUp[key] = true;

			KeyUpEvent e(key);
			window->m_WindowData.EventCallbackFn(e);
		} break;
		case WM_LBUTTONDOWN:
		{
			Input::s_MouseButtonDown[Mouse::ButtonLeft] = true;

			MouseButtonDownEvent e(Mouse::ButtonLeft);
			window->m_WindowData.EventCallbackFn(e);
		} break;
		case WM_MBUTTONDOWN:
		{
			Input::s_MouseButtonDown[Mouse::ButtonMiddle] = true;

			MouseButtonDownEvent e(Mouse::ButtonMiddle);
			window->m_WindowData.EventCallbackFn(e);
		} break;
		case WM_RBUTTONDOWN:
		{
			Input::s_MouseButtonDown[Mouse::ButtonRight] = true;

			MouseButtonDownEvent e(Mouse::ButtonRight);
			window->m_WindowData.EventCallbackFn(e);
		} break;
		case WM_LBUTTONUP:
		{
			Input::s_MouseButtonUp[Mouse::ButtonLeft] = true;

			MouseButtonUpEvent e(Mouse::ButtonLeft);
			window->m_WindowData.EventCallbackFn(e);
		} break;
		case WM_MBUTTONUP:
		{
			Input::s_MouseButtonUp[Mouse::ButtonMiddle] = true;

			MouseButtonUpEvent e(Mouse::ButtonMiddle);
			window->m_WindowData.EventCallbackFn(e);
		} break;
		case WM_RBUTTONUP:
		{
			Input::s_MouseButtonUp[Mouse::ButtonRight] = true;

			MouseButtonUpEvent e(Mouse::ButtonRight);
			window->m_WindowData.EventCallbackFn(e);
		} break;
		case WM_MOUSEMOVE:
		{
			float x = LOWORD(lParam);
			float y = HIWORD(lParam);

			Input::s_MouseX = x;
			Input::s_MouseY = y;

			MouseMoveEvent e(x, y);
			window->m_WindowData.EventCallbackFn(e);
		} break;
		case WM_MOUSEWHEEL:
		{
			MouseScrollEvent e(0, GET_WHEEL_DELTA_WPARAM(wParam));
			window->m_WindowData.EventCallbackFn(e);
		} break;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	Window::Window(int width, int height, const std::string& title)
	{
		m_WindowData.Closed = false;
		m_WindowData.Title = title;
		m_WindowData.Width = width;
		m_WindowData.Height = height;
		m_WindowData.EventCallbackFn = nullptr;

		if (!CreateWin32Window())
			DIE("Could not create window");
		else
			LOG_INFO("Window successfully created");

		// Hard code OpenGL 3.3 for now (TODO: Make OpenGL version customizable)
		if (!CreateOpenGLContext(3, 3))
			DIE("Could not create OpenGL context");
		else
			LOG_INFO("OpenGL context successfully created");

		m_SwapIntervalFn = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

		m_WindowTable[m_WindowHandle] = this;

		ShowWindow(m_WindowHandle, SW_SHOW);
	}

	Window::~Window()
	{
		HDC windowDC = GetDC(m_WindowHandle);
		wglMakeCurrent(windowDC, NULL);
		wglDeleteContext(m_OpenGLRenderingContext);

		UnregisterClass(m_WindowData.Title.c_str(), GetModuleHandle(NULL));
		DestroyWindow(m_WindowHandle);
	}

	void Window::Update()
	{
		SwapBuffers(GetDC(m_WindowHandle));
	}

	void Window::PollEvents()
	{
		MSG msg;

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	bool Window::CreateWin32Window()
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
		
		m_WindowHandle = CreateWindowEx(WS_EX_LEFT,
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
		GetClientRect(m_WindowHandle, &clientRect);
		m_WindowData.Width = clientRect.right - clientRect.left;
		m_WindowData.Height = clientRect.bottom - clientRect.top;
	
		return m_WindowHandle != NULL;
	}

	bool Window::CreateOpenGLContext(int major, int minor)
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

		HDC dc =  GetDC(m_WindowHandle);

		int pixel_format;
		UINT num_formats;
		wglChoosePixelFormatARB(dc, pixelFormatAttribs, 0, 1, &pixel_format, &num_formats);
		if (!num_formats)
			DIE("Failed to set the OpenGL pixel format.");

		PIXELFORMATDESCRIPTOR pfd;
		DescribePixelFormat(dc, pixel_format, sizeof(pfd), &pfd);
		if (!SetPixelFormat(dc, pixel_format, &pfd))
			DIE("Failed to set the OpenGL pixel format.");

		// Specify that we want to create an OpenGL 3.3 core profile context
		int gl_attribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, major,
			WGL_CONTEXT_MINOR_VERSION_ARB, minor,
			WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0,
		};

		HGLRC glContext = wglCreateContextAttribsARB(dc, 0, gl_attribs);
		if (!glContext)
			DIE("Failed to create OpenGL context.");

		if (!wglMakeCurrent(dc, glContext))
			DIE("Failed to activate OpenGL rendering context.");

		return true;
	}

	bool Window::InitOpenGLContext()
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

	HWND Window::CreateDummyWindow()
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

	void Window::LoadOpenGLExtensions()
	{
		wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)utils::OpenGLLoader::LoadFunction("wglCreateContextAttribsARB");
		wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFOTMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	}

	Window* Window::GetWindowByHandle(const HWND& handle)
	{
		if (m_WindowTable.find(handle) == m_WindowTable.end())
			return nullptr;

		return m_WindowTable.at(handle);
	}
}