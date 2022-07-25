#include "Window.h"

#include "core/Log.h"

namespace monk
{
	std::unordered_map<HWND, Window*> Window::m_WindowTable;

	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// LOG_INFO("Callback { Window: {0}, Message: {1}, wParam {2}, lParam: {3}", hWnd, uMsg, wParam, lParam);
		Window* window = Window::GetWindowByHandle(hWnd);

		if (!window)
			return DefWindowProc(hWnd, uMsg, wParam, lParam);

		switch (uMsg)
		{
		case WM_CLOSE:
			window->Close();
			break;
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	Window::Window(int width, int height, const std::string& title)
	{
		m_WindowData.Closed = false;
		m_WindowData.Initialized = false;
		m_WindowData.Title = title;
		m_WindowData.Width = width;
		m_WindowData.Height = height;

		WNDCLASSEX wc = { };

		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = title.c_str();

		if (!RegisterClassEx(&wc))
			DIE("Could not register '{0}' window class", title);
		else
			LOG_INFO("Window class '{0}' registered", title);

		m_WindowHandle = CreateWindowEx(WS_EX_LEFT,
			title.c_str(),
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

		m_WindowTable[m_WindowHandle] = this;
		m_WindowData.Initialized = true;

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
	}

	Window* Window::GetWindowByHandle(const HWND& handle)
	{
		if (m_WindowTable.find(handle) == m_WindowTable.end())
			return nullptr;

		return m_WindowTable.at(handle);
	}
}