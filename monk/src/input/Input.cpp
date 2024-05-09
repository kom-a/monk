#include "input/Input.h"

#include <windows.h>

namespace monk
{
	float Input::s_MouseX = 0;
	float Input::s_MouseY = 0;
	bool Input::s_KeyDown[Key::MaxCount];
	bool Input::s_KeyUp[Key::MaxCount];
	bool Input::s_MouseButtonDown[Mouse::MaxCount];
	bool Input::s_MouseButtonUp[Mouse::MaxCount];

	bool Input::IsKeyPressed(KeyCode key)
	{
		return GetKeyState((int)key) & (1 << 15);
	}

	bool Input::IsKeyDown(KeyCode key)
	{
		return s_KeyDown[key];
	}

	bool Input::IsKeyUp(KeyCode key)
	{
		return s_KeyUp[key];
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		return GetKeyState((int)button) & (1 << 15);
	}

	bool Input::IsMouseButtonDown(MouseCode button)
	{
		return s_MouseButtonDown[button];
	}

	bool Input::IsMouseButtonUp(MouseCode button)
	{
		return s_MouseButtonUp[button];
	}

	mmath::vec2 Input::GetMousePosition()
	{
		return mmath::vec2(s_MouseX, s_MouseY);
	}

	float Input::GetMouseX()
	{
		return s_MouseX;
	}

	float Input::GetMouseY()
	{
		return s_MouseY;
	}

	void Input::SetMousePosition(int x, int y)
	{
		s_MouseX = x;
		s_MouseY = y;
	}

	void Input::Update()
	{
		for (int i = 0; i < Key::MaxCount; i++)
		{
			s_KeyDown[i] = false;
			s_KeyUp[i] = false;
		}

		for (int i = 0; i < Mouse::MaxCount; i++)
		{
			s_MouseButtonDown[i] = false;
			s_MouseButtonUp[i] = false;
		}
	}
}