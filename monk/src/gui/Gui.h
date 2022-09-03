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
	struct GuiStyle
	{
		math::vec2 Padding;

		float HeaderHeight;
		math::vec4 HeaderColor;
		math::vec4 HotHeaderColor;
		math::vec4 ActiveHeaderColor;

		math::vec4 WindowBackgroundColor;
		math::vec4 HotWindowBackgroundColor;
		math::vec4 ActiveWindowBackgroundColor;
		math::vec2 WindowBorderSize;
		math::vec4 WindowBorderColor;
		
		math::vec4 ButtonColor;
		math::vec4 HotButtonColor;
		math::vec4 ActiveButtonColor;

		math::vec2 WindowResizeCornerSize;
		math::vec4 WindowResizeCornerColor;
		math::vec4 HotWindowResizeCornerColor;
		math::vec4 ActiveWindowResizeCornerColor;
	};

	class Gui
	{
	public:
		static void Init();
		static GuiStyle& GetStyle();

		static void NewFrame(const math::mat4& projection);
		static void EndFrame();

		static void Begin(const std::string& name);
		static void End();

	private:
		static void RestoreWindow(GuiWindow* window);

		static void DrawWindow(const GuiWindow* window);

	private:
		static math::vec2 s_LastMouse;
		static GuiStyle s_GuiStyle;
		static std::unordered_map<uint32_t, GuiWindowCacheData> s_WindowCache;
		static std::unique_ptr<Renderer2D> s_Renderer;
	};
}