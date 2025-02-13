#include "MUI.h"

#include <windows.h>
#include <cassert>
#include <array>
#include <unordered_map>

#include <MOGL/MOGL.h>
#include <MFL/MFL.h>

#include "Render/Renderer.h"

namespace mui
{
	IMPL_LOGGER("MUI");

	struct Style
	{
		Vec4f WindowBackgroudColor			= Vec4f(0.20f, 0.20f, 0.20f, 0.9f);
		Vec4f WindowBackgroudHotColor		= Vec4f(0.20f, 0.20f, 0.20f, 0.9f);
		Vec4f WindowTitlebarColor			= Vec4f(0.10f, 0.10f, 0.10f, 1.0f);
		Vec4f WindowTitlebarHotColor		= Vec4f(0.10f, 0.10f, 0.20f, 1.0f);
		Vec4f WindowBorderColor				= Vec4f(0.10f, 0.10f, 0.10f, 1.0f);
		Vec4f WindowResizeColor				= Vec4f(0.80f, 0.80f, 0.80f, 1.0f);
		Vec4f WindowCloseButtonColor		= Vec4f(0.60f, 0.10f, 0.10f, 1.0f);
		Vec4f WindowCloseButtonHotColor		= Vec4f(0.70f, 0.20f, 0.20f, 1.0f);
		Vec4f WindowCollapseButtonColor		= Vec4f(0.50f, 0.50f, 0.50f, 1.0f);
		Vec4f WindowCollapseButtonHotColor	= Vec4f(0.60f, 0.60f, 0.60f, 1.0f);

		Vec4f ButtonColor					= Vec4f(0.10f, 0.10f, 0.30f, 1.0f);
		Vec4f ButtonHotColor				= Vec4f(0.15f, 0.15f, 0.30f, 1.0f);
		Vec4f ButtonActiveColor				= Vec4f(0.30f, 0.30f, 0.60f, 1.0f);

		Vec4f CheckboxColor					= Vec4f(0.10f, 0.10f, 0.30f, 1.0f);
		Vec4f CheckboxHotColor				= Vec4f(0.15f, 0.15f, 0.30f, 1.0f);
		Vec4f CheckboxActiveColor			= Vec4f(0.65f, 0.65f, 0.70f, 1.0f);
		Vec2f CheckboxSize					= Vec2f(20.0f, 20.0f);
		float CheckboxActivePadding			= 3.0f;

		Vec4f RadioColor					= Vec4f(0.10f, 0.10f, 0.30f, 1.0f);
		Vec4f RadioHotColor					= Vec4f(0.15f, 0.15f, 0.30f, 1.0f);
		Vec4f RadioActiveColor				= Vec4f(0.65f, 0.65f, 0.70f, 1.0f);
		float RadioDiameter					= 20.0f;
		float RadioActivePadding			= 6.0f;

		Vec4f ScrollbarColor				= Vec4f(0.15f, 0.15f, 0.20f, 0.9f);
		Vec4f ScrollColor					= Vec4f(0.40f, 0.40f, 0.50f, 1.0f);
		Vec4f ScrollHotColor				= Vec4f(0.45f, 0.45f, 0.55f, 1.0f);

		Vec2f ButtonSize					= Vec2f(150.0f, 50.0f);

		float WindowTitlebarHeight			= 24.0f;
		float WindowTitlebarItem			= 0.6f;
		float WindowTitlebarItemPaddings	= 4.0f;
		float WindowBorderThickness			= 2.0f;
		float WindowResizeThickness			= 16.0f;
		float WindowPadding					= 8.0f;

		float WindowTitleFontSize			= 14.0f;
		float ContentFontSize				= 12.0f;

		float WindowScrollbarWidth			= 16.0f;
	};

	struct Window;

	struct Context
	{
		HWND Handle = nullptr;

		Vec2f NextFirstWindowCreationPosition = Vec2f(100.0f, 100.0f);

		std::unordered_map<std::string, Window> Windows;
		std::list<Window*> WindowsDrawOrder;
		Window* CurrentWindow	= nullptr;
		Window* DraggingWindow	= nullptr;
		Window* ResizingWindow	= nullptr;
		Window* ScrollingWindow = nullptr;
		Window* HotWindow		= nullptr;
		
		int* RadioButtonValue = nullptr;

		std::vector<Vertex> DrawBuffer;
	};

	static Context				g_Context;
	static Style				g_Style;
	static Input				g_Input;
	static Renderer*			g_Renderer;

	struct Window
	{
		std::string	Name;
		bool Active;
		bool Collapsed;
		bool* Open;

		uint32_t Order;

		Vec2f Position = Vec2f(100, 100);
		Vec2f Size = Vec2f(400, 400);

		float TitlebarHeight = 12;

		float ContentHeight = 0;

		Vec2f Cursor = Vec2f(0, 0);

		float Scroll = 0.0f;

		DrawList WindowDrawList;
		DrawList ContentDrawList;

		bool TitlebarHot() const
		{
			const auto& x = g_Input.MousePosition.X;
			const auto& y = g_Input.MousePosition.Y;

			const auto mx = Position.X + Size.X;
			const auto my = Position.Y + g_Style.WindowTitlebarHeight;

			return x >= Position.X && x <= mx && y >= Position.Y && y <= my;
		}

		bool BodyHot() const
		{
			if (Collapsed)
				return false;

			const auto& x = g_Input.MousePosition.X;
			const auto& y = g_Input.MousePosition.Y;

			const auto mx = Position.X + Size.X;
			const auto my = Position.Y + Size.Y;

			return x >= Position.X && x <= mx &&
				y >= Position.Y + g_Style.WindowTitlebarHeight && y <= my + g_Style.WindowTitlebarHeight;
		}

		bool ResizeHot() const
		{
			const auto& x = g_Input.MousePosition.X;
			const auto& y = g_Input.MousePosition.Y;

			const auto rx = Position.X + Size.X - g_Style.WindowResizeThickness;
			const auto ry = Position.Y + Size.Y + g_Style.WindowTitlebarHeight - g_Style.WindowResizeThickness;

			return x >= rx && x <= rx + g_Style.WindowResizeThickness && y >= ry && y <= ry + g_Style.WindowResizeThickness;
		}

		bool CollapseHot() const
		{
			const Vec2f itemSize = Vec2f(
				g_Style.WindowTitlebarItem * g_Style.WindowTitlebarHeight,
				g_Style.WindowTitlebarItem * g_Style.WindowTitlebarHeight);

			Vec2f position = Vec2f(
				Position.X + Size.X - g_Style.WindowTitlebarItemPaddings - itemSize.X,
				Position.Y + (g_Style.WindowTitlebarHeight - itemSize.Y) * 0.5f);

			if (Open)
				position.X -= itemSize.X + g_Style.WindowTitlebarItemPaddings;

			const auto& x = g_Input.MousePosition.X;
			const auto& y = g_Input.MousePosition.Y;

			const auto rx = position.X + itemSize.X;
			const auto ry = position.Y + itemSize.Y;

			return x >= position.X && x <= rx && y >= position.Y && y <= ry;
		}

		bool CloseHot() const
		{
			if (!Open)
				return false;

			const Vec2f itemSize = Vec2f(
				g_Style.WindowTitlebarItem * g_Style.WindowTitlebarHeight,
				g_Style.WindowTitlebarItem * g_Style.WindowTitlebarHeight);

			Vec2f position = Vec2f(
				Position.X + Size.X - g_Style.WindowTitlebarItemPaddings - itemSize.X,
				Position.Y + (g_Style.WindowTitlebarHeight - itemSize.Y) * 0.5f);

			const auto& x = g_Input.MousePosition.X;
			const auto& y = g_Input.MousePosition.Y;

			const auto rx = position.X + itemSize.X;
			const auto ry = position.Y + itemSize.Y;

			return x >= position.X && x <= rx && y >= position.Y && y <= ry;
		}

		bool Hot() const
		{
			return TitlebarHot() || BodyHot();
		}

		Vec2f ContentSize()
		{
			if (ContentHeight > Size.Y + g_Style.WindowTitlebarHeight)
			{
				return Vec2f(Size.X - g_Style.WindowScrollbarWidth, Size.Y);
			}
			else
			{
				return Size;
			}
		}
	};

	static void ClampWindowInViewport(Window* window)
	{
		const auto& viewportWidth = g_Input.Viewport.X;
		const auto& viewportHeight = g_Input.Viewport.Y;

		if (viewportHeight == 0)
			return;

		const float padding = 25;

		if (window->Position.X > viewportWidth)
			window->Position.X = viewportWidth - padding;

		if (window->Position.Y > viewportHeight)
			window->Position.Y = viewportHeight - padding;
	}

	void InitForWin32(void* handle)
	{
		g_Context.Handle = (HWND)handle;

		g_Renderer = new Renderer();
	}

	void Shutdown()
	{
		g_Context.Windows.clear();
		delete g_Renderer;
	}

	Input& GetInput()
	{
		return g_Input;
	}

	static std::string ClampTextToWidth(const std::string& text, const std::string& postfix, float maxWidth, float fontSize, const MFL::Font& font, Vec2f* size = nullptr)
	{
		Vec2f postfixSize = g_Renderer->CalcTextSize(postfix, fontSize);
		std::stringstream ss;

		float x = 0;
		bool clamped = false;

		float scale = font.GetScaleForFontSize(fontSize);
		MFL::VerticalMetrics hm = font.GetVerticalMetrics();

		Vec2f sz{ 0.0f, hm.LineHeight * scale };

		for (uint32_t c : text)
		{
			const MFL::GlyphData& glyphData = font.GetGlyphDataByUnicode(c);
			float width = glyphData.Width * scale;
			float height = glyphData.Height * scale;
			float ascender = glyphData.Ascender * scale;
			float descender = glyphData.Descender * scale;
			float advance = glyphData.Advance == 0 ? width : glyphData.Advance * scale;

			if (c == '\n')
			{
				sz.X = max(sz.X, x);
				sz.Y += (hm.LineHeight + hm.LineGap) * scale;

				x = 0;
				clamped = false;
				ss << (char)c;
				continue;
			}

			if (x + advance + postfixSize.X > maxWidth)
			{
				clamped = true;
				ss << postfix;
				x = 0;
			}
			else if (!clamped)
			{
				ss << (char)c;
				x += advance;
			}
		}

		if (size)
			*size = sz;

		return ss.str();
	}

	static std::string WrapTextToWidth(const std::string& text, float maxWidth, float fontSize, const MFL::Font& font, Vec2f* size = nullptr)
	{
		std::stringstream ss;

		float x = 0;

		Vec2f sz{ 0.0f, 0.0f };

		for (uint32_t c : text)
		{
			const MFL::GlyphData& glyphData = font.GetGlyphDataByUnicode(c);
			float scale = font.GetScaleForFontSize(fontSize);
			float width = glyphData.Width * scale;
			float height = glyphData.Height * scale;
			float ascender = glyphData.Ascender * scale;
			float descender = glyphData.Descender * scale;
			float advance = glyphData.Advance == 0 ? width : glyphData.Advance * scale;

			if (c == '\n')
			{
				MFL::VerticalMetrics hm = font.GetVerticalMetrics();

				sz.X = max(sz.X, x);
				sz.Y += (hm.LineHeight + hm.LineGap) * scale;

				x = 0;
				ss << (char)c;
				continue;
			}

			if (x + advance > maxWidth)
			{
				MFL::VerticalMetrics hm = font.GetVerticalMetrics();

				sz.X = max(sz.X, x);
				sz.Y += (hm.LineHeight + hm.LineGap) * scale;

				ss << '\n';
				x = 0;
			}

			ss << (char)c;
			x += advance;
		}

		if (size)
			*size = sz;

		return ss.str();
	}

	struct WrapTextWord
	{
		std::string Data;
		Vec2f Size = Vec2f(0.0f, 0.0f);
	};

	static std::string WordWrapTextToWidth(const std::string& text, float maxWidth, float fontSize, const MFL::Font& font, Vec2f* size)
	{
		std::vector<WrapTextWord> words;
		std::stringstream ss;

		float x = 0.0f;
		const std::string_view delimeters = " .,;\n";

		for (uint32_t c : text)
		{
			const MFL::GlyphData& glyphData = font.GetGlyphDataByUnicode(c);
			float scale = font.GetScaleForFontSize(fontSize);
			float width = glyphData.Width * scale;
			float height = glyphData.Height * scale;
			float ascender = glyphData.Ascender * scale;
			float descender = glyphData.Descender * scale;
			float advance = glyphData.Advance == 0 ? width : glyphData.Advance * scale;

			if (delimeters.find((char)c, 0) != std::string_view::npos)
			{
				ss << (char)c;
				x += advance;

				WrapTextWord word;
				word.Data = ss.str();
				word.Size.X = x;
				words.emplace_back(word);

				ss.str("");
				x = 0.0f;
			}
			else
			{
				ss << (char)c;
				x += advance;
			}
		}

		if (ss.str().size())
		{
			WrapTextWord word;
			word.Data = ss.str();
			word.Size.X = x;
			words.emplace_back(word);
		}

		x = 0;
		ss.str("");
		
		float scale = font.GetScaleForFontSize(fontSize);
		MFL::VerticalMetrics hm = font.GetVerticalMetrics();
		Vec2f sz{ 0.0f, hm.LineHeight * scale };

		for (WrapTextWord& word : words)
		{
			if (word.Data.size() && word.Data.back() == '\n')
			{
				sz.X = max(sz.X, x);
				sz.Y += (hm.LineHeight + hm.LineGap) * scale;

				x = 0;
			}

			if (word.Size.X > maxWidth)
				word.Data = ClampTextToWidth(word.Data, "...", maxWidth, fontSize, font);

			if (x + word.Size.X > maxWidth)
			{
				float scale = font.GetScaleForFontSize(fontSize);
				MFL::VerticalMetrics hm = font.GetVerticalMetrics();
				sz.X = max(sz.X, x);
				sz.Y += (hm.LineHeight + hm.LineGap) * scale;

				ss << '\n';
				x = 0;
			}

			ss << word.Data;
			x += word.Size.X;
		}

		if (size)
			*size = sz;

		return ss.str();
	}

	static bool Inside(Vec2f position, Vec2f size, Vec2f point)
	{
		float x = point.X;
		float y = point.Y;
		
		float rx = position.X + size.X;
		float ry = position.Y + size.Y;

		return x >= position.X && x <= rx && y >= position.Y && y <= ry;
	}

	static bool InsideCircle(Vec2f center, float radius, Vec2f point)
	{
		Vec2f delta = Vec2f(point.X - center.X, point.Y - center.Y);

		return std::sqrt(delta.X * delta.X + delta.Y * delta.Y) < radius;
	}

	static bool MouseInside(Vec2f position, Vec2f size)
	{
		Vec2f point = g_Input.MousePosition;
		point.Y += g_Context.CurrentWindow->Scroll;

		return Inside(position, size, point) && g_Context.CurrentWindow == g_Context.HotWindow && !g_Context.CurrentWindow->TitlebarHot();
	}

	static bool MouseInsideCircle(Vec2f center, float radius)
	{
		Vec2f point = g_Input.MousePosition;
		point.Y += g_Context.CurrentWindow->Scroll;	

		return InsideCircle(center, radius, point) && g_Context.CurrentWindow == g_Context.HotWindow && !g_Context.CurrentWindow->TitlebarHot();
	}

	static bool MouseClicked(Vec2f position, Vec2f size)
	{
		Vec2f downPoint = g_Input.MouseDownPosition;
		downPoint.Y += g_Context.CurrentWindow->Scroll;

		Vec2f releasePoint = g_Input.MouseReleasePosition;
		releasePoint.Y += g_Context.CurrentWindow->Scroll;

		return Inside(position, size, downPoint) && 
			Inside(position, size, releasePoint) && 
			g_Context.CurrentWindow == g_Context.HotWindow && 
			!g_Context.CurrentWindow->TitlebarHot();
	}

	static bool MouseClickedCircle(Vec2f center, float radius)
	{
		Vec2f downPoint = g_Input.MouseDownPosition;
		downPoint.Y += g_Context.CurrentWindow->Scroll;

		Vec2f releasePoint = g_Input.MouseReleasePosition;
		releasePoint.Y += g_Context.CurrentWindow->Scroll;

		return InsideCircle(center, radius, downPoint) && 
			InsideCircle(center, radius, releasePoint) && 
			g_Context.CurrentWindow == g_Context.HotWindow &&
			!g_Context.CurrentWindow->TitlebarHot();
	}

	static void DrawWindowTitlebar(Window& window)
	{
		DrawList& drawList = window.WindowDrawList;

		const Vec2f titlebarSize = Vec2f(window.Size.X, g_Style.WindowTitlebarHeight);
		const Vec4f titlebarColor = (g_Context.HotWindow && g_Context.HotWindow == &window) ? g_Style.WindowTitlebarHotColor : g_Style.WindowTitlebarColor;

		// Draw titlebar background
		drawList.DrawQuad(window.Position, titlebarSize, titlebarColor);

		const float itemSize = titlebarSize.Y * g_Style.WindowTitlebarItem;
		const Vec2f titlebarItemSize = Vec2f(itemSize, itemSize);

		// Draw close button
		const Vec2f closeButtonPosition = Vec2f(
			window.Position.X + window.Size.X - g_Style.WindowTitlebarItemPaddings - itemSize,
			window.Position.Y + (titlebarSize.Y - itemSize) * 0.5f);

		if (window.Open != nullptr)
		{
			const Vec4f closeButtonColor = window.CloseHot() ? g_Style.WindowCloseButtonHotColor : g_Style.WindowCloseButtonColor;
			drawList.DrawQuad(closeButtonPosition, titlebarItemSize, closeButtonColor);
		}

		// Draw collapse button
		Vec4f collapseButtonColor = window.CollapseHot() ? g_Style.WindowCollapseButtonHotColor : g_Style.WindowCollapseButtonColor;
		Vec2f collapseButtonPosition = closeButtonPosition;
		if (window.Open != nullptr)
			collapseButtonPosition.X -= titlebarItemSize.X + g_Style.WindowTitlebarItemPaddings;
		drawList.DrawQuad(collapseButtonPosition, titlebarItemSize, collapseButtonColor);

		float titlebarButtonsWidth = window.Open == nullptr ? titlebarItemSize.X : 2 * titlebarItemSize.X;
		const float fontSize = g_Style.WindowTitleFontSize;

		const std::string title = ClampTextToWidth(window.Name, "...", window.Size.X - 2 * g_Style.WindowPadding - titlebarButtonsWidth, fontSize, *g_Renderer->GetFont());
		drawList.DrawString(title, Vec2f(
			window.Position.X + g_Style.WindowPadding,
			window.Position.Y + g_Style.WindowTitlebarHeight - fontSize / 2), fontSize, Vec4f(1.0f, 1.0f, 1.0f, 1.0f), *g_Renderer->GetFont());
	}

	static void DrawWindow(Window& window)
	{
		if (!window.Active)
			return;

		DrawWindowTitlebar(window);

		if (window.Collapsed)
			return;	

		DrawList& drawList = window.WindowDrawList;

		const Vec2f titlebarSize = Vec2f(window.Size.X, g_Style.WindowTitlebarHeight);

		// Draw window background
		const Vec2f windowClientPosition = Vec2f(window.Position.X, window.Position.Y + titlebarSize.Y);
		const Vec4f backgroundColor = (g_Context.HotWindow && g_Context.HotWindow == &window) ? g_Style.WindowBackgroudHotColor : g_Style.WindowBackgroudColor;
		drawList.DrawQuad(windowClientPosition, window.Size, backgroundColor);

		//drawList.DrawQuad(window.Position + Vec2f(0.0f, g_Style.WindowTitlebarHeight), window.ContentSize(), Vec4f(0.0f, 0.0f, 1.0f, 1.0f));

		// Draw window scrollbar
		if (window.ContentHeight > window.Size.Y + g_Style.WindowTitlebarHeight)
		{
			float tbHeight = g_Style.WindowTitlebarHeight;
			float sbWidth = g_Style.WindowScrollbarWidth;
			Vec2f scrollbarPosition = Vec2f(window.Position.X, window.Position.Y + tbHeight) + Vec2f(window.Size.X - sbWidth, 0.0f);
			Vec2f scrollbarSize = Vec2f(sbWidth, window.Size.Y);

			drawList.DrawQuad(scrollbarPosition, scrollbarSize, g_Style.ScrollbarColor);

			const float scrollOffset = 2.0f;
			float contentDisplayRatio = window.Size.Y / window.ContentHeight;
			float scrollRatio = window.Scroll / window.Size.Y;

			Vec2f scrollSize = Vec2f(scrollbarSize.X - scrollOffset * 2 - g_Style.WindowBorderThickness / 2, scrollbarSize.Y * contentDisplayRatio);
			Vec2f scrollPosition = scrollbarPosition + Vec2f(scrollOffset, scrollSize.Y * scrollRatio);
			bool hot = Inside(scrollPosition, scrollSize, g_Input.MousePosition);
			Vec4f color = hot ? g_Style.ScrollHotColor : g_Style.ScrollColor;

			drawList.DrawQuad(scrollPosition, scrollSize, color);

			if (Inside(scrollPosition, scrollSize, g_Input.MouseDownPosition))
			{
				g_Context.ScrollingWindow = &window;
			}
		}

		// Draw window resize
		Vec2f resizePosition = Vec2f(
			window.Position.X + window.Size.X - g_Style.WindowResizeThickness,
			window.Position.Y + window.Size.Y + g_Style.WindowTitlebarHeight - g_Style.WindowResizeThickness);
		Vec2f resizeSize = Vec2f(g_Style.WindowResizeThickness, g_Style.WindowResizeThickness);
		drawList.DrawQuad(resizePosition, resizeSize, g_Style.WindowResizeColor);

		// Draw window borders
		const Vec2f leftBorderPosition = Vec2f(window.Position.X, window.Position.Y + titlebarSize.Y);
		const Vec2f leftBorderSize = Vec2f(g_Style.WindowBorderThickness, window.Size.Y);

		const Vec2f bottomBorderPosition = Vec2f(window.Position.X, window.Position.Y + titlebarSize.Y + window.Size.Y - g_Style.WindowBorderThickness);
		const Vec2f bottomBorderSize = Vec2f(window.Size.X, g_Style.WindowBorderThickness);

		const Vec2f rightBorderPosition = Vec2f(leftBorderPosition.X + window.Size.X - g_Style.WindowBorderThickness, leftBorderPosition.Y);
		const Vec2f rightBorderSize = Vec2f(g_Style.WindowBorderThickness, window.Size.Y);

		drawList.DrawQuad(leftBorderPosition, leftBorderSize, g_Style.WindowBorderColor);
		drawList.DrawQuad(bottomBorderPosition, bottomBorderSize, g_Style.WindowBorderColor);
		drawList.DrawQuad(rightBorderPosition, rightBorderSize, g_Style.WindowBorderColor);
	}

	void NewFrame()
	{
		g_Input.MouseLeftClicked = g_Input.MouseLeftDown && !g_Input.LastMouseLeftDown;
		g_Input.MouseLeftReleased = !g_Input.MouseLeftDown && g_Input.LastMouseLeftDown;
		if (g_Input.MouseLeftReleased)
			g_Input.MouseReleasePosition = g_Input.MousePosition;

		for (auto& [name, window] : g_Context.Windows)
		{
			window.Active = false;
		}

		g_Context.WindowsDrawOrder.clear();
	}

	void EndFrame()
	{
		Window* clickedWindow	= nullptr;
		Window* hotWindow		= nullptr;
		Window* draggingWindow	= nullptr;
		Window* resizingWindow	= nullptr;
		Window* collapsingWindow = nullptr;
		Window* closingWindow	= nullptr;

		for (auto& [name, window] : g_Context.Windows)
		{
			if(!window.Active)
				continue;

			if (window.Hot() && g_Input.MouseLeftClicked)
			{
				if (clickedWindow)
				{
					if (window.Order > clickedWindow->Order)
						clickedWindow = &window;
				}
				else 
					clickedWindow = &window;
			}

			g_Context.WindowsDrawOrder.push_back(&window);
		}

		if (clickedWindow)
			clickedWindow->Order = (uint32_t)-1;

		g_Context.WindowsDrawOrder.sort([](const Window* left, const Window* right) {
				return left->Order < right->Order;
			});

		uint32_t order = 0;
		for (auto window : g_Context.WindowsDrawOrder)
		{
			ClampWindowInViewport(window);

			if (window->TitlebarHot() && g_Input.MouseLeftClicked)
				draggingWindow = window;

			if (window->ResizeHot() && g_Input.MouseLeftClicked)
				resizingWindow = window;

			if (window->CollapseHot() && g_Input.MouseLeftClicked)
				collapsingWindow = window;

			if (window->CloseHot() && g_Input.MouseLeftClicked)
				closingWindow = window;

			if (window->Hot())
				hotWindow = window;

			window->Order = order++;
		}

		g_Context.HotWindow = hotWindow;

		if (!g_Input.MouseLeftDown)
		{
			g_Context.DraggingWindow = nullptr;
			g_Context.ResizingWindow = nullptr;
		}

		if (clickedWindow != draggingWindow)
			draggingWindow = nullptr;
		if (clickedWindow != resizingWindow)
			resizingWindow = nullptr;
		if (clickedWindow != collapsingWindow)
			collapsingWindow = nullptr;
		if (clickedWindow != closingWindow)
			closingWindow = nullptr;

		if (g_Context.DraggingWindow && g_Context.ResizingWindow)
		{
			if (g_Context.DraggingWindow->Order > g_Context.ResizingWindow->Order)
				g_Context.ResizingWindow = nullptr;
			else
				g_Context.DraggingWindow = nullptr;
		}
		
		if (g_Context.DraggingWindow)
		{
			Vec2f delta = g_Input.MousePosition - g_Input.LastMousePosition;

			g_Context.DraggingWindow->Position = Vec2f(g_Context.DraggingWindow->Position.X + delta.X,
				g_Context.DraggingWindow->Position.Y + delta.Y);
		}

		if (g_Context.ResizingWindow && !g_Context.ResizingWindow->Collapsed)
		{
			Vec2f delta = g_Input.MousePosition - g_Input.LastMousePosition;

			g_Context.ResizingWindow->Size = Vec2f(g_Context.ResizingWindow->Size.X + delta.X,
				g_Context.ResizingWindow->Size.Y + delta.Y);

			if (g_Context.ResizingWindow->Size.X < g_Style.WindowResizeThickness * 4)
				g_Context.ResizingWindow->Size.X = g_Style.WindowResizeThickness * 4;
				
			if (g_Context.ResizingWindow->Size.Y < g_Style.WindowResizeThickness * 2)
				g_Context.ResizingWindow->Size.Y = g_Style.WindowResizeThickness * 2;
		}

		if (collapsingWindow)
			collapsingWindow->Collapsed = !collapsingWindow->Collapsed;
		else if (closingWindow)
			*closingWindow->Open = false;
		else
		{
			if (draggingWindow && !g_Context.DraggingWindow)
				g_Context.DraggingWindow = draggingWindow;

			if (resizingWindow && !g_Context.ResizingWindow)
				g_Context.ResizingWindow = resizingWindow;
		}

		if (g_Context.ScrollingWindow && !g_Context.DraggingWindow)
		{
			float contentHeight = max(g_Context.ScrollingWindow->ContentHeight - g_Context.ScrollingWindow->Size.Y - g_Style.WindowTitlebarHeight, 0.0f);

			g_Context.ScrollingWindow->Scroll += g_Input.MouseDelta.Y * (g_Context.ScrollingWindow->ContentHeight / g_Context.ScrollingWindow->Size.Y);
			g_Context.ScrollingWindow->Scroll = std::clamp(g_Context.ScrollingWindow->Scroll, 0.0f, contentHeight);
		}

		if (g_Context.HotWindow)
		{
			float contentHeight = max(g_Context.HotWindow->ContentHeight - g_Context.HotWindow->Size.Y - g_Style.WindowTitlebarHeight, 0.0f);

			g_Context.HotWindow->Scroll -= g_Input.Scroll.Y * g_Input.ScrollSize;
			g_Context.HotWindow->Scroll = std::clamp(g_Context.HotWindow->Scroll, 0.0f, contentHeight);
		}

		g_Input.MouseDelta = g_Input.MousePosition - g_Input.LastMousePosition;

		g_Input.LastMouseLeftDown = g_Input.MouseLeftDown;
		g_Input.LastMousePosition = g_Input.MousePosition;

		if (g_Input.MouseLeftClicked)
			g_Input.MouseDownPosition = g_Input.MousePosition;
		if (g_Input.MouseLeftReleased)
		{
			g_Input.MouseDownPosition = { -1.0f, -1.0f };
			g_Context.ScrollingWindow = nullptr;
		}
		
		g_Input.MouseReleasePosition = { -1.0f, -1.0f };
		g_Input.Scroll = { 0.0f, 0.0f };
	}

	void Begin(const std::string& name, bool* open)
	{
		auto itWnd = g_Context.Windows.find(name);
		if (itWnd != g_Context.Windows.end())
		{
			// Restore window
			g_Context.CurrentWindow = &itWnd->second;
		}
		else
		{
			// Create new window
			Window newWindow;
			newWindow.Name = name;
			newWindow.Position = g_Context.NextFirstWindowCreationPosition;
			newWindow.Size = Vec2f(300, 300);
			newWindow.Order = g_Context.Windows.size() + 1;
			newWindow.Collapsed = false;
			newWindow.TitlebarHeight = g_Style.WindowTitlebarHeight;
			g_Context.Windows.try_emplace(name, newWindow);

			g_Context.CurrentWindow = &g_Context.Windows.find(name)->second;

			g_Context.NextFirstWindowCreationPosition += Vec2f(50, 50);
		}

		g_Context.CurrentWindow->Active = open ? *open : true;
		g_Context.CurrentWindow->Open	= open;

		g_Context.CurrentWindow->Cursor.X = g_Style.WindowPadding;
		g_Context.CurrentWindow->Cursor.Y = g_Style.WindowTitlebarHeight + g_Style.WindowPadding;

		g_Context.CurrentWindow->ContentDrawList.SetYOffset(-g_Context.CurrentWindow->Scroll);

		DrawWindow(*g_Context.CurrentWindow);
	}

	void End()
	{
		g_Context.CurrentWindow->ContentHeight = g_Context.CurrentWindow->Cursor.Y;
		g_Context.CurrentWindow = nullptr;
	}

	void Separator()
	{
		if (!g_Context.CurrentWindow)
			return;

		if (g_Context.CurrentWindow->Collapsed)
			return;

		Window& window = *g_Context.CurrentWindow;
		DrawList& drawList = g_Context.CurrentWindow->ContentDrawList;
		Vec2f contentSize = window.ContentSize();

		Vec2f position = window.Position + window.Cursor;
		Vec2f size(contentSize.X - g_Style.WindowPadding * 2, 1.0f);

		drawList.DrawQuad(position, size, Vec4f(0.1f, 0.1f, 0.1f, 1.0f));

		window.Cursor.X = g_Style.WindowPadding;
		window.Cursor.Y += g_Style.WindowPadding;
	}

	void Text(const std::string& text, uint32_t flags)
	{
		if (!g_Context.CurrentWindow)
			return;

		if (g_Context.CurrentWindow->Collapsed)
			return;

		Window& window = *g_Context.CurrentWindow;
		DrawList& drawList = window.ContentDrawList;
		Vec2f contentSize = window.ContentSize();
		float fontSize = g_Style.ContentFontSize;
		std::string textTmp = text;

		Vec2f cursorOffset{ 0.0f, 0.0f };
		
		if (flags & TextFlags::TextFlags_Clamp)
			textTmp = ClampTextToWidth(text, "...", contentSize.X - g_Style.WindowPadding * 2, fontSize, *g_Renderer->GetFont(), &cursorOffset);
		else if (flags & TextFlags::TextFlags_Wrap)
			textTmp = WrapTextToWidth(text, contentSize.X - g_Style.WindowPadding * 2, fontSize, *g_Renderer->GetFont(), &cursorOffset);
		else if (flags & TextFlags::TextFlags_WordWrap)
			textTmp = WordWrapTextToWidth(text, contentSize.X - g_Style.WindowPadding * 2, fontSize, *g_Renderer->GetFont(), &cursorOffset);
		else
			cursorOffset = g_Renderer->CalcTextSize(text, fontSize);

		Vec2f textPosition(
			window.Position.X + window.Cursor.X, 
			window.Position.Y + window.Cursor.Y + fontSize);

		drawList.DrawString(textTmp, textPosition, fontSize, Vec4f(1.0f, 1.0f, 1.0f, 1.0f), *g_Renderer->GetFont());

		window.Cursor.X = g_Style.WindowPadding;
		window.Cursor.Y += cursorOffset.Y + g_Style.WindowPadding;
	}

	bool Button(const std::string& text, Vec2f size, uint32_t flags)
	{
		if (!g_Context.CurrentWindow)
			return false;

		if (g_Context.CurrentWindow->Collapsed)
			return false;

		Window& window = *g_Context.CurrentWindow;
		Vec2f& cursor = window.Cursor;
		DrawList& drawList = window.ContentDrawList;
		float fontSize = g_Style.ContentFontSize;

		std::string textTmp = text;

		Vec2f buttonPosition = g_Context.CurrentWindow->Position + g_Context.CurrentWindow->Cursor;
		Vec2f buttonSize = size == Vec2f() ? Vec2f(100, 25) : size;

		Vec2f textSize;

		if (flags & ButtonFlags_AutoFit)
		{
			textSize = g_Renderer->CalcTextSize(text, fontSize);
			buttonSize = textSize;
			buttonSize += Vec2f(g_Style.WindowPadding * 2, g_Style.WindowPadding);
		}
		else
		{
			textTmp = ClampTextToWidth(text, "...", buttonSize.X - g_Style.WindowPadding, fontSize, *g_Renderer->GetFont());
			textSize = g_Renderer->CalcTextSize(textTmp, fontSize);
		}

		bool hot = MouseInside(buttonPosition, buttonSize);
		bool active = hot && Inside(buttonPosition, buttonSize, Vec2f(g_Input.MouseDownPosition.X, g_Input.MouseDownPosition.Y + window.Scroll));

		Vec4f color = hot ? g_Style.ButtonHotColor : g_Style.ButtonColor;

		if (active)
			color = g_Style.ButtonActiveColor;

		drawList.DrawQuad(buttonPosition, buttonSize, color);

		Vec2f textPosition = buttonPosition + Vec2f(buttonSize.X / 2, buttonSize.Y / 2) - Vec2f(textSize.X / 2, textSize.Y / 2) + Vec2f(0.0f, fontSize);
		drawList.DrawString(textTmp, textPosition, fontSize, Vec4f(1.0f, 1.0f, 1.0f, 1.0f), *g_Renderer->GetFont());

		g_Context.CurrentWindow->Cursor.X = g_Style.WindowPadding;
		g_Context.CurrentWindow->Cursor.Y += buttonSize.Y + g_Style.WindowPadding;

		return MouseClicked(buttonPosition, buttonSize);
	}

	void Checkbox(const std::string& text, bool* value)
	{
		if (!g_Context.CurrentWindow)
			return;

		if (g_Context.CurrentWindow->Collapsed)
			return;

		Window& window = *g_Context.CurrentWindow;
		Vec2f& cursor = window.Cursor;
		DrawList& drawList = window.ContentDrawList;
		Vec2f contentSize = window.ContentSize();

		Vec2f checkboxPosition = window.Position + window.Cursor;
		Vec2f checkboxSize = g_Style.CheckboxSize;
		
		bool hot = MouseInside(checkboxPosition, checkboxSize);
		Vec4f color = hot ? g_Style.CheckboxColor : g_Style.CheckboxHotColor;

		drawList.DrawQuad(checkboxPosition, checkboxSize, color);

		if (value && *value)
		{
			const float& offset = g_Style.CheckboxActivePadding;
			Vec2f activePosition = Vec2f(checkboxPosition.X + offset, checkboxPosition.Y + offset);
			Vec2f activeSize = g_Style.CheckboxSize - Vec2f(offset, offset) * 2;

			drawList.DrawQuad(activePosition, activeSize, g_Style.CheckboxActiveColor);
		}

		float fontSize = g_Style.ContentFontSize;
		Vec2f textSize;
		const std::string textTmp = ClampTextToWidth(text, "...", contentSize.X - checkboxSize.X - g_Style.WindowPadding * 2, fontSize, *g_Renderer->GetFont(), &textSize);
		Vec2f textPosition = checkboxPosition + Vec2f(checkboxSize.X + g_Style.WindowPadding, fontSize + g_Style.CheckboxSize.Y * 0.5f - textSize.Y * 0.5f);
		drawList.DrawString(textTmp, textPosition, fontSize, Vec4f(1.0f, 1.0f, 1.0f, 1.0f), *g_Renderer->GetFont());

		if (value && MouseClicked(checkboxPosition, checkboxSize))
			*value = !*value;

		cursor.X = g_Style.WindowPadding;
		cursor.Y += checkboxSize.Y + g_Style.WindowPadding;
	}

	void BeginRadio(int* value)
	{
		if (g_Context.RadioButtonValue)
			return;

		g_Context.RadioButtonValue = value;
	}

	void Radio(const std::string& text, int index)
	{
		if (!g_Context.CurrentWindow)
			return;

		if (g_Context.CurrentWindow->Collapsed)
			return;

		Window& window = *g_Context.CurrentWindow;
		Vec2f& cursor = window.Cursor;
		DrawList& drawList = window.ContentDrawList;
		Vec2f contentSize = window.ContentSize();
		const float& diameter = g_Style.RadioDiameter;
		const float radius = diameter / 2;

		Vec2f radioboxCenter = window.Position + window.Cursor + Vec2f(g_Style.RadioDiameter / 2, g_Style.RadioDiameter / 2);

		bool hot = MouseInsideCircle(radioboxCenter, radius);
		Vec4f color = hot ? g_Style.RadioColor : g_Style.RadioHotColor;

		drawList.DrawCircle(radioboxCenter, g_Style.RadioDiameter, color);

		if (g_Context.RadioButtonValue && *g_Context.RadioButtonValue == index)
		{
			Vec2f activeCenter = Vec2f(radioboxCenter.X, radioboxCenter.Y);
			float activeDiameter = g_Style.RadioDiameter - g_Style.RadioActivePadding;

			drawList.DrawCircle(activeCenter, activeDiameter, g_Style.RadioActiveColor);
		}

		float fontSize = g_Style.ContentFontSize;
		Vec2f textSize;
		const std::string textTmp = ClampTextToWidth(text, "...", contentSize.X - diameter - g_Style.WindowPadding * 2, fontSize, *g_Renderer->GetFont(), &textSize);
		Vec2f textPosition = radioboxCenter + Vec2f(radius + g_Style.WindowPadding, fontSize - radius + g_Style.RadioDiameter * 0.5f - textSize.Y * 0.5f);
		drawList.DrawString(textTmp, textPosition, fontSize, Vec4f(1.0f, 1.0f, 1.0f, 1.0f), *g_Renderer->GetFont());

		if (MouseClickedCircle(radioboxCenter, radius))
			*g_Context.RadioButtonValue = index;

		cursor.X = g_Style.WindowPadding;
		cursor.Y += g_Style.RadioDiameter + g_Style.WindowPadding;
	}

	void EndRadio()
	{
		if (!g_Context.RadioButtonValue)
			return;

		g_Context.RadioButtonValue = nullptr;
	}

	void Render()
	{
		for (Window* window : g_Context.WindowsDrawOrder)
		{
			g_Renderer->SetViewport(0, 0, g_Input.Viewport.X, g_Input.Viewport.Y);

			g_Renderer->SetWindowViewport(window->Position.X, window->Position.Y, window->Size.X, window->Size.Y + window->TitlebarHeight);
			g_Renderer->FlushDrawList(window->WindowDrawList);

			Vec2f contentSize = window->ContentSize();

			g_Renderer->SetWindowViewport(window->Position.X, window->Position.Y + g_Style.WindowTitlebarHeight, contentSize.X, contentSize.Y);
			g_Renderer->FlushDrawList(window->ContentDrawList);
			
			window->WindowDrawList.Clear();
			window->ContentDrawList.Clear();
		}
	}
}

