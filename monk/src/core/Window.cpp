#include "Window.h"

#include "core/Log.h"

#include <gl/glew.h>
#include <string>

namespace monk
{
	std::unordered_map<HWND, Window*> Window::m_WindowTable;

	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Window* window = Window::GetWindowByHandle(hWnd);

		if (!window)
			return DefWindowProc(hWnd, uMsg, wParam, lParam);

		switch (uMsg)
		{
		case WM_CLOSE:
		{
			window->Close();
		}break;
		case WM_SIZE:
		{
			window->m_WindowData.Width = LOWORD(lParam);
			window->m_WindowData.Height = HIWORD(lParam);
			int height = HIWORD(lParam);
			glViewport(0, 0, window->m_WindowData.Width, window->m_WindowData.Height);
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

		if (!CreateWin32Window())
			DIE("Could not create window");
		else
			LOG_INFO("Window successfully created");

		if (!CreateOpenGLContext())
			DIE("Could not create OpenGL context");
		else
			LOG_INFO("OpenGL version: {0}", (char*)glGetString(GL_VERSION));
		
		if (glewInit() != GLEW_OK)
			LOG_ERROR("Fauled to init glew");
		else
			LOG_INFO("GLEW version: {0}", glewGetString(GLEW_VERSION));

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
		MSG msg;

		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		SwapBuffers(GetDC(m_WindowHandle));
	}

	bool Window::CreateWin32Window()
	{
		WNDCLASSEX wc = { };

		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = GetModuleHandle(NULL);
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