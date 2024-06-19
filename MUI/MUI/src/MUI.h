#pragma once

#include <Windows.h>
#include <string>

#include <WangMauna/WangMauna.h>

namespace mui
{
	DEFINE_LOGGER();

	struct Input
	{
		int MouseX;
		int MouseY;
		bool MouseLeftDown;
		bool MouseLeftClicked;

		bool LastMouseLeftDown;
		int LastMouseX;
		int LastMouseY;
	};

	void InitForWin32(HWND handle);
	void Shutdown();

	Input& GetInput();

	void NewFrame();
	void EndFrame();

	void Begin(const std::string& name);
	void End();

	void Render();
}