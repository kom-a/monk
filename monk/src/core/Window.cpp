#include "Window.h"

#include "core/Log.h"

namespace monk
{
	LRESULT CALLBACK winproc(HWND hwnd, UINT wm, WPARAM wp, LPARAM lp)
	{
		return DefWindowProc(hwnd, wm, wp, lp);
	}

	Window::Window(int width, int height, const std::string& title)
	{
		WNDCLASSEX wc = { };
	
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = winproc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = m_WindowClassName.c_str();

		if (!RegisterClassEx(&wc))
			DIE("Could not register window class");
		else
			LOG_INFO("Window class '{0}' registered", m_WindowClassName);

		m_WindowHandle = CreateWindowEx(WS_EX_LEFT,
			m_WindowClassName.c_str(),
			title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			width,
			height,	
			NULL,
			NULL,
			GetModuleHandle(NULL),
			NULL);

		if (!m_WindowHandle)
			DIE("Could not create window");
		else
			LOG_INFO("Window successfully created");

		ShowWindow(m_WindowHandle, SW_SHOW);
	}

	Window::~Window()
	{
		UnregisterClass(m_WindowClassName.c_str(), GetModuleHandle(NULL));
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
	}

}