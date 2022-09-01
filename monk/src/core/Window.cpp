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

		if (!CreateOpenGLContext())
			DIE("Could not create OpenGL context");

		m_SwapIntervalFn = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

		m_WindowTable[m_WindowHandle] = this;

		ShowWindow(m_WindowHandle, SW_SHOW);
	}

	Window::~Window()
	{
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

		int windowStyle = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

		wc.cbSize = sizeof(wc);
		wc.style = windowStyle;
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

	bool Window::CreateOpenGLContext()
	{
		HDC windowDC = GetDC(m_WindowHandle);
		
		PIXELFORMATDESCRIPTOR desiredPixelFormat = { };
		desiredPixelFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		desiredPixelFormat.nVersion = 1;
		desiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
		desiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
		desiredPixelFormat.cColorBits = 24;
		desiredPixelFormat.cAlphaBits = 8;
		desiredPixelFormat.iLayerType = PFD_MAIN_PLANE;

		int suggestedPixelFormatIndex = ChoosePixelFormat(windowDC, &desiredPixelFormat);
		PIXELFORMATDESCRIPTOR suggestedPixelFormat;
		DescribePixelFormat(windowDC, suggestedPixelFormatIndex, sizeof(suggestedPixelFormat), &suggestedPixelFormat);
		SetPixelFormat(windowDC, suggestedPixelFormatIndex, &suggestedPixelFormat);

		HGLRC openGLRC = wglCreateContext(windowDC);		
		return wglMakeCurrent(windowDC, openGLRC);
	}

	Window* Window::GetWindowByHandle(const HWND& handle)
	{
		if (m_WindowTable.find(handle) == m_WindowTable.end())
			return nullptr;

		return m_WindowTable.at(handle);
	}
}