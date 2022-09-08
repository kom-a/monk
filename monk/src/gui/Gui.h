#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "gui/GuiWindow.h"

#include "graphics/Renderer2D.h"
#include "math/Math.h"

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

			HotWindowCollapseButton,
			WindowCollapseButton,

			HotWindowCloseButton,
			WindowCloseButton,

			WindowResizeCorner,
			HotWindowResizeCorner,
			ActiveWindowResizeCorner,

			Count
		};
	};

	struct GuiStyle
	{
		math::vec2 Padding;
		float HeaderHeight;
		float HeaderPadding;

		math::vec4 Colors[GuiColor::Count];

		math::vec2 MinWindowSize;
		math::vec2 WindowBorderSize;

		math::vec2 WindowResizeCornerSize;
	};

	class Gui
	{
	public:
		static void Init();
		static GuiStyle& GetStyle();

		static void NewFrame(const math::mat4& projection);
		static void EndFrame();

		static void Begin(const std::string& name, bool* open = nullptr);
		static void End();

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
		static math::vec2 s_LastMouse;
		static GuiStyle s_GuiStyle;
		static std::unordered_map<uint32_t, GuiWindowCacheData> s_WindowCache;
		static std::unique_ptr<Renderer2D> s_Renderer;
	};
}