#pragma once

#include <MML/MML.h>
#include "input/KeyCodes.h"
#include "input/MouseCodes.h"

#include <windows.h>

class Application;

namespace monk
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsKeyDown(KeyCode key);
		static bool IsKeyUp(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static bool IsMouseButtonDown(MouseCode button);
		static bool IsMouseButtonUp(MouseCode button);
		static mml::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

		static void SetMousePosition(int x, int y);

		static void Update();

	private:
		static float s_MouseX, s_MouseY;

		static bool s_KeyDown[Key::MaxCount];
		static bool s_KeyUp[Key::MaxCount];
		static bool s_MouseButtonDown[Mouse::MaxCount];
		static bool s_MouseButtonUp[Mouse::MaxCount];

		friend class Application;
	};
}
