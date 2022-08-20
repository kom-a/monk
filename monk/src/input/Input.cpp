#include "input/Input.h"

#include <windows.h>

namespace monk
{
	float Input::s_MouseX = 0;
	float Input::s_MouseY = 0;

	bool Input::IsKeyPressed(KeyCode key)
	{
		return GetKeyState((int)key) & (1 << 15);
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		return GetKeyState((int)button) & (1 << 15);
	}

	math::vec2 Input::GetMousePosition()
	{
		return math::vec2(s_MouseX, s_MouseY);
	}

	float Input::GetMouseX()
	{
		return s_MouseX;
	}

	float Input::GetMouseY()
	{
		return s_MouseY;
	}

}