#pragma once

#include <string>
#include <list>
#include <memory>

#include "Renderer.h"

namespace mui
{
	struct Window
	{
		explicit Window(const std::string& name);

		std::string Name;
		Vec2i Position = { 100, 100 };
		Vec2i Size = { 300, 200 };
	};

	struct Viewport
	{
		Viewport() = default;
		Viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		uint32_t X		= 0;
		uint32_t Y		= 0;
		uint32_t Width	= 0;
		uint32_t Height = 0;
	};

	struct Input
	{
		static int MouseX;
		static int MouseY;

		static bool IsMouseDown;
	};

	class Mui
	{
	public:
		static void Init();
		static void ShutDown();

		static void NewFrame(const Viewport& viewport);
		static void EndFrame();

		static void Begin(const std::string& name);
		static void End();

		static bool Button(std::string_view text);
	private:
		static void DrawWindow(const Window& window);

	private:
		struct State
		{
			Window* CurrentWindow = nullptr;
		};

		static std::list<Window> s_Windows;
		static State s_State;

		static Viewport s_Viewport;
		static std::unique_ptr<Renderer> s_Renderer;
	};

}