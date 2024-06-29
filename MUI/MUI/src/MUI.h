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

	void Begin(const std::string& name, bool* open = nullptr);
	void End();

	// Widgets
	void SeparateLine();
	void Text(const std::string& text);
	bool Button(const std::string& text);
	void Checkbox(const std::string& text, bool* value);
	void Radio(const std::string& text, int* value, int index);
	void SliderInt(const std::string& text, int* value, int min, int max);
	void SliderFloat(const std::string& text, float* value, float min, float max);
	void DragInt(const std::string& text, int* value);
	void DragFloat(const std::string& text, float* value);
	

	void Render();
}