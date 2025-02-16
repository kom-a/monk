#pragma once

#include <string>

#include <WangMauna/WangMauna.h>

#include "Render/Math.h"

namespace mui
{
	DEFINE_LOGGER();

	struct Input
	{
		Vec2f MousePosition;
		Vec2f LastMousePosition;
		Vec2f MouseDelta;

		bool MouseLeftDown;
		bool MouseLeftClicked;
		bool MouseLeftReleased;
		bool LastMouseLeftDown;

		Vec2f Viewport;

		Vec2f MouseDownPosition = { -1.0f, -1.0f };
		Vec2f MouseReleasePosition = { -1.0f, -1.0f };

		Vec2f Scroll = { 0.0f, 0.0f };
		float ScrollSize = 25.0f;
	};
	
	enum TextFlags
	{
		TextFlags_None		= 0,
		TextFlags_Clamp		= 1 << 1,
		TextFlags_Wrap		= 1 << 2,
		TextFlags_WordWrap	= 1 << 3,
	};

	enum ButtonFlags
	{
		ButtonFlags_None		= 0,
		ButtonFlags_AutoFit		= 1 << 1
	};

	void InitForWin32(void* handle);
	void Shutdown();

	Input& GetInput();

	void NewFrame();
	void EndFrame();

	void Begin(const std::string& name, bool* open = nullptr);
	void End();

	// Widgets
	void Separator();
	void Text(const std::string& text, uint32_t flags = TextFlags::TextFlags_None);
	bool Button(const std::string& text, Vec2f size = Vec2f(), uint32_t flags = ButtonFlags::ButtonFlags_None);
	void Checkbox(const std::string& text, bool* value);

	void BeginRadio(int* value);
	void Radio(const std::string& text, int index);
	void EndRadio();

	void SliderInt (const std::string& text, int* value  , int min = -2147483648, int max = 2147483647);
	void SliderInt2(const std::string& text, int value[2], int min = -2147483648, int max = 2147483647);
	void SliderInt3(const std::string& text, int value[3], int min = -2147483648, int max = 2147483647);
	void SliderInt4(const std::string& text, int value[4], int min = -2147483648, int max = 2147483647);

	void SliderFloat (const std::string& text, float* value  , float step, float min, float max);
	void SliderFloat2(const std::string& text, float value[2], float step, float min, float max);
	void SliderFloat3(const std::string& text, float value[3], float step, float min, float max);
	void SliderFloat4(const std::string& text, float value[4], float step, float min, float max);

	void DragInt (const std::string& text, int* value  , int step = 1, int min = -2147483648, int max = 2147483647);
	void DragInt2(const std::string& text, int value[2], int step = 1, int min = -2147483648, int max = 2147483647);
	void DragInt3(const std::string& text, int value[3], int step = 1, int min = -2147483648, int max = 2147483647);
	void DragInt4(const std::string& text, int value[4], int step = 1, int min = -2147483648, int max = 2147483647);


	void DragFloat (const std::string& text, float* value, float step = 0.2f, float min = -FLT_MAX, float max = FLT_MAX);
	void DragFloat2(const std::string& text, float* value, float step = 0.2f, float min = -FLT_MAX, float max = FLT_MAX);
	void DragFloat3(const std::string& text, float* value, float step = 0.2f, float min = -FLT_MAX, float max = FLT_MAX);
	void DragFloat4(const std::string& text, float* value, float step = 0.2f, float min = -FLT_MAX, float max = FLT_MAX);
	
	void Render();
}