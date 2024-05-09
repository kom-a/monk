#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "gui/GuiWindow.h"

#include "graphics/Renderer2D.h"
#include "graphics/OrthographicCamera.h"
#include <MMath/MMath.h>

#include "events/MouseEvent.h"



namespace monk
{
	namespace GuiColor
	{
		enum
		{
			None = 0,

			Header,
			HotHeader,
			ActiveHeader,

			Window,
			HotWindow,
			ActiveWindow,

			WindowBorder,
			WindowTitle,

			HotWindowCollapseButton,
			WindowCollapseButton,

			HotWindowCloseButton,
			WindowCloseButton,

			WindowResizeCorner,
			HotWindowResizeCorner,
			ActiveWindowResizeCorner,

			Button,
			
			Count
		};
	};

	struct GuiStyle
	{
		mmath::vec2 Padding;
		float HeaderHeight;
		float HeaderPadding;
		float HeaderContentSize; // Text, buttons... occupation in percentage (0.5 - content occupy 50% of HeaderHeight)

		mmath::vec4 Colors[GuiColor::Count];

		mmath::vec2 MinWindowSize;
		mmath::vec2 WindowBorderSize;

		mmath::vec2 WindowResizeCornerSize;
	};

	class Gui
	{
	public:
		static void Init();
		static GuiStyle& GetStyle();

		static void NewFrame();
		static void EndFrame();

		static void Begin(const std::string& name, bool* open = nullptr);
		static void End();

		static void Button(const std::string& text, const mmath::vec2& size = mmath::vec2(100, 20));

	private:
		static void DefaultStyle();
		static void DefaultColorTheme();
		static bool RestoreWindow(GuiWindow* window);
		static void BeginWindow(GuiWindow* window);
		static bool IsWindowHot(GuiWindow* window);

		static void DrawWindow(const GuiWindow* window);
		static void DrawWindowHeader(const GuiWindow* window);
		static void DrawWindowBody(const GuiWindow* window);
		
	private:
		static mmath::vec2 s_LastMouse;
		static GuiStyle s_GuiStyle;
		static std::unordered_map<uint32_t, GuiWindowCacheData> s_WindowCache;
		static OrthographicCamera s_Camera;
		static std::unique_ptr<Renderer2D> s_Renderer;
	};
}