#include "gui/Gui.h"

#include "input/Input.h"
#include "core/Log.h"
#include "core/Assert.h"

#include <functional>

namespace monk
{
	GuiStyle Gui::s_GuiStyle;
	std::unordered_map<uint32_t, GuiWindowCacheData> Gui::s_WindowCache;
	std::unique_ptr<Renderer2D> Gui::s_Renderer;

	struct
	{
		GuiWindow* HeadWindow;
		GuiWindow* TailWindow;
		int WindowsCount;

		GuiWindow* CurrentWindow;
		GuiWindow* HotWindow;
		int HotWindowZOrder;

		uint32_t HotWindowID;
		uint32_t ActiveWindowID;

		uint32_t MovingWindowID;
		uint32_t ResizingWindowID;

		uint32_t CollapsedWindowID;
		uint32_t ClosedWindowID;

		math::vec2 LastMouse;

		void Reset()
		{
			HeadWindow = nullptr;
			TailWindow = nullptr;
			WindowsCount = 0;
			CurrentWindow = nullptr;
			HotWindow = nullptr;
			HotWindowZOrder = -1;
			ActiveWindowID = 0;
			HotWindowID = 0;
			HotWindowZOrder = -1;
			MovingWindowID = 0;
			ResizingWindowID = 0;
			CollapsedWindowID = 0;
			ClosedWindowID = 0;
		}

		void AddWindowToList(GuiWindow* window)
		{
			if (!HeadWindow)
			{
				InsertFirst(window);
			}
			else
			{
				GuiWindow* current = HeadWindow;

				while (current)
				{
					if (!current->Next && current->ZOrder <= window->ZOrder)
					{
						InsertLast(window);
						break;
					}

					if (current->ZOrder > window->ZOrder)
					{
						InsertBefore(window, current);
						break;
					}

					current = current->Next;
				}
			}

			WindowsCount++;
		}

		void ClearWindowList()
		{
			GuiWindow* window = HeadWindow;

			while (window)
			{
				GuiWindow* next = window->Next;
				delete window;
				window = next;
			}

			HeadWindow = nullptr;
			TailWindow = nullptr;
			WindowsCount = 0;
		}

	private:
		void InsertBefore(GuiWindow* insertWindow, GuiWindow* base)
		{
			MONK_ASSERT(insertWindow, "insertWindow was null");
			MONK_ASSERT(base, "base window was null");

			if (base == HeadWindow)
			{
				insertWindow->Next = base;
				base->Prev = insertWindow;
				insertWindow->Prev = nullptr;
				HeadWindow = insertWindow;
			}
			else
			{
				base->Prev->Next = insertWindow;
				insertWindow->Prev = base->Prev;
				base->Prev = insertWindow;
				insertWindow->Next = base;
			}
		}

		void InsertAfter(GuiWindow* insertWindow, GuiWindow* base)
		{
			MONK_ASSERT(insertWindow, "insertWindow was null");
			MONK_ASSERT(base, "base window was null");

			if (base == TailWindow)
			{
				base->Next = insertWindow;
				insertWindow->Prev = base;
				insertWindow->Next = nullptr;
				TailWindow = insertWindow;
			}
			else
			{
				base->Next->Prev = insertWindow;
				insertWindow->Next = base->Next;
				base->Next = insertWindow;
				insertWindow->Prev = base;
			}
		}

		void InsertFirst(GuiWindow* window)
		{
			MONK_ASSERT(window, "window was null");

			if (!HeadWindow)
			{
				HeadWindow = window;
				TailWindow = window;
				window->Prev = nullptr;
				window->Next = nullptr;
			}
			else
			{
				HeadWindow->Prev = window;
				window->Next = HeadWindow;
				HeadWindow = window;
				window->Prev = nullptr;
			}
		}

		void InsertLast(GuiWindow* window)
		{
			MONK_ASSERT(window, "window was null");

			if (!HeadWindow)
			{
				HeadWindow = window;
				TailWindow = window;
				window->Prev = nullptr;
				window->Next = nullptr;
			}
			else
			{
				window->Prev = TailWindow;
				TailWindow->Next = window;
				TailWindow = window;
				window->Next = nullptr;
			}
		}
	} GuiState;

	void Gui::Init()
	{
		s_Renderer = std::make_unique<Renderer2D>();

		Gui::DefaultStyle();

		GuiState.Reset();
		GuiState.LastMouse = Input::GetMousePosition();
	}

	GuiStyle& Gui::GetStyle()
	{
		return s_GuiStyle;
	}

	void Gui::NewFrame(const math::mat4& projection)
	{
		GuiState.HotWindowID = 0;
		GuiState.HotWindow = nullptr;
		GuiState.HotWindowZOrder = -1;
		GuiState.CollapsedWindowID = 0;
		GuiState.ClosedWindowID = 0;
		s_Renderer->Begin(projection);
	}

	void Gui::EndFrame()
	{
		GuiWindow* current = GuiState.HeadWindow;

		int ZOrder = 0;

		while (current)
		{
			current->ZOrder = ZOrder++;

			if (current->ID == GuiState.ResizingWindowID)
			{
				GuiStyle& style = Gui::GetStyle();
				current->Size += Input::GetMousePosition() - GuiState.LastMouse;
				current->Size = math::Clamp(current->Size, style.MinWindowSize, current->Size);
			}
			else if (current->ID == GuiState.MovingWindowID)
				current->Position += Input::GetMousePosition() - GuiState.LastMouse;

			if (current->ID == GuiState.CollapsedWindowID)
				current->Collapsed = !current->Collapsed;
			if (current->Open && current->ID == GuiState.ClosedWindowID)
				*current->Open = false;

			s_WindowCache[current->ID] = current->GetCacheData();

			
			DrawWindow(current);
			current = current->Next;
		}

		s_Renderer->End();
		GuiState.ClearWindowList();
		GuiState.LastMouse = Input::GetMousePosition();

		if (!Input::IsMouseButtonPressed(Mouse::ButtonLeft))
		{
			GuiState.MovingWindowID = 0;
			GuiState.ResizingWindowID = 0;
		}
	}

	void Gui::Begin(const std::string& name, bool* open)
	{
		MONK_ASSERT(!GuiState.CurrentWindow, "Can not 'Begin' new window inside another one. Call 'End' before starting new window.");

		GuiWindow* window = new GuiWindow(name, open);
		Gui::RestoreWindow(window);

		GuiState.CurrentWindow = window;
		
		Gui::BeginWindow(window);

		GuiState.AddWindowToList(window);
	}

	void Gui::End()
	{
		MONK_ASSERT(GuiState.CurrentWindow, "No GuiWindow to 'End'");

		GuiState.CurrentWindow = nullptr;
	}

	void Gui::Button(const std::string& text, const math::vec2& size)
	{
		MONK_ASSERT(GuiState.CurrentWindow, "No current window. Use 'Begin' before calling 'Button'");

		const auto& style = GetStyle();

		math::vec2 position = GuiState.CurrentWindow->Position + GuiState.CurrentWindow->CursorPosition + math::vec2(0.0f, style.Padding.y);

		s_Renderer->FillRect(position, size, style.Colors[GuiColor::Button]);

		GuiState.CurrentWindow->CursorPosition += math::vec2(0.0f, size.y + style.Padding.y);
	}

	//////////////////////////////////////////////////////////////////////////
	// Private
	//////////////////////////////////////////////////////////////////////////

	void Gui::DefaultStyle()
	{
		GuiStyle& style = Gui::GetStyle();

		style.Padding = math::vec2(5.0f, 5.0f);
		style.HeaderHeight = 24.0f;
		style.HeaderPadding = 6.0f;
		style.HeaderContentSize = 0.8f;
		style.FontSize = style.HeaderHeight * 0.75;
		style.MinWindowSize = math::vec2(100, 25);
		style.WindowBorderSize = math::vec2(1.0f);
		style.WindowResizeCornerSize = math::vec2(14.0f);

		Gui::DefaultColorTheme();
	}

	void Gui::DefaultColorTheme()
	{
		GuiStyle& style = Gui::GetStyle();
		auto& colors = style.Colors;

		colors[GuiColor::None] = math::vec4(0.0f);
		
		colors[GuiColor::Header] = math::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		colors[GuiColor::HotHeader] = math::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		colors[GuiColor::ActiveHeader] = math::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		
		colors[GuiColor::Window] = math::vec4(0.15f, 0.15f, 0.15f, 1.0f);
		colors[GuiColor::HotWindow] = math::vec4(0.15f, 0.15f, 0.15f, 1.0f);
		colors[GuiColor::ActiveWindow] = math::vec4(0.15f, 0.15f, 0.15f, 1.0f);

		colors[GuiColor::WindowTitle] = math::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		colors[GuiColor::WindowBorder] = math::vec4(0.35f, 0.35f, 0.35f, 1.0f);
		
		colors[GuiColor::WindowCollapseButton] = math::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		colors[GuiColor::HotWindowCollapseButton] = math::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		colors[GuiColor::WindowCloseButton] = math::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		colors[GuiColor::HotWindowCloseButton] = math::vec4(0.85f, 0.3f, 0.3f, 1.0f);

		colors[GuiColor::WindowResizeCorner] = math::vec4(0.1f, 0.2f, 0.3f, 1.0f);
		colors[GuiColor::HotWindowResizeCorner] = math::vec4(0.3f, 0.7f, 0.2f, 1.0f);
		colors[GuiColor::ActiveWindowResizeCorner] = math::vec4(0.2f, 0.4f, 0.1f, 1.0f);

		colors[GuiColor::Button] = math::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	}

	bool Gui::RestoreWindow(GuiWindow* window)
	{
		if (s_WindowCache.find(window->ID) != s_WindowCache.end())
		{
			const GuiWindowCacheData& cache = s_WindowCache.at(window->ID);
			window->Restore(cache);

			return true;
		}

		return false;
	}

	void Gui::BeginWindow(GuiWindow* window)
	{
		if (Gui::IsWindowHot(window))
		{
			GuiState.HotWindowID = window->ID;
			GuiState.HotWindow = window;
			GuiState.HotWindowZOrder = window->ZOrder;

			if (Input::IsMouseButtonDown(Mouse::ButtonLeft))
			{
				GuiState.ActiveWindowID = window->ID;
				window->ZOrder = (uint32_t)-1;

				if (window->IsResizeHot())
				{
					GuiState.ResizingWindowID = window->ID;
					GuiState.MovingWindowID = 0;
				}
				else
				{
					GuiState.MovingWindowID = window->ID;
					GuiState.ResizingWindowID = 0;
				}

				if (window->IsCollapseHot())
					GuiState.CollapsedWindowID = window->ID;
				if (window->IsCloseHot())
					GuiState.ClosedWindowID = window->ID;
			}
		}
	}

	bool Gui::IsWindowHot(GuiWindow* window)
	{
		if (!window->IsHot() || GuiState.ResizingWindowID)
			return false;

		if (GuiState.HotWindow && window->ZOrder < GuiState.HotWindowZOrder)
			return false;

		return true;
	}

	void Gui::DrawWindow(const GuiWindow* window)
	{
		DrawWindowHeader(window);
		
		if(!window->Collapsed)
			DrawWindowBody(window);
	}

	void Gui::DrawWindowHeader(const GuiWindow* window)
	{
		GuiStyle& style = Gui::GetStyle();
		const math::vec2 headerSize = math::vec2(window->Size.x, style.HeaderHeight);
		GuiRect collapseButtonRect = window->GetCollapseButton();
		GuiRect closeButtonRect = window->GetCloseButton();
		GuiRect border;
		border.Size = headerSize + style.WindowBorderSize * 2;
		border.Position = window->Position - style.WindowBorderSize;
		math::vec2 titlePosition = collapseButtonRect.Position + collapseButtonRect.Size + math::vec2(style.HeaderPadding * 2, 0.0f);

		math::vec4 headerColor;
		
		math::vec4 collapseButtonColor = window->IsCollapseHot() ? style.Colors[GuiColor::HotWindowCollapseButton] : style.Colors[GuiColor::WindowCollapseButton];
		math::vec4 closeButtonColor = window->IsCloseHot() ? style.Colors[GuiColor::HotWindowCloseButton] : style.Colors[GuiColor::WindowCloseButton];

		if (window->ID == GuiState.ActiveWindowID)
		{
			headerColor = style.Colors[GuiColor::ActiveHeader];
		}
		else if (window->ID == GuiState.HotWindowID)
		{
			headerColor = style.Colors[GuiColor::HotHeader];
		}
		else
		{
			headerColor = style.Colors[GuiColor::Header];
		}

		s_Renderer->FillRect(border.Position, border.Size, style.Colors[GuiColor::WindowBorder]);
		s_Renderer->FillRect(window->Position, headerSize, headerColor);

		s_Renderer->FillRect(collapseButtonRect.Position, collapseButtonRect.Size, collapseButtonColor);
		s_Renderer->Text(titlePosition, window->Name, style.FontSize, style.Colors[GuiColor::WindowTitle]);
		
		if (window->Open)
		{
			s_Renderer->FillRect(closeButtonRect.Position, closeButtonRect.Size, closeButtonColor);
			s_Renderer->Text(closeButtonRect.Position + math::vec2(style.FontSize * 0.435, closeButtonRect.Size.y - 2), "x", style.FontSize * 1.5f);
		}
	}

	void Gui::DrawWindowBody(const GuiWindow* window)
	{
		GuiStyle& style = Gui::GetStyle();

		const math::vec2& headerPosition = window->Position;
		math::vec2 headerSize = math::vec2(window->Size.x, style.HeaderHeight);
		math::vec2 windowBodyPosition = math::vec2(window->Position.x, window->Position.y + style.HeaderHeight);
		const math::vec2& windowBodySize = window->Size;
		math::vec2 borderPosition = window->Position + math::vec2(0, style.HeaderHeight) - math::vec2(style.WindowBorderSize.y, 0.0f);
		math::vec2 borderSize = window->Size + math::vec2(style.WindowBorderSize.x * 2, style.WindowBorderSize.y);
		math::vec2 resizeCornerPosition = window->GetResizeCorner().Position;

		math::vec4 headerColor;
		math::vec4 windowColor;

		if (window->ID == GuiState.ActiveWindowID)
		{
			headerColor = style.Colors[GuiColor::ActiveHeader];
			windowColor = style.Colors[GuiColor::ActiveWindow];
		}
		else if (window->ID == GuiState.HotWindowID)
		{
			headerColor = style.Colors[GuiColor::HotHeader];
			windowColor = style.Colors[GuiColor::HotWindow];
		}
		else
		{
			headerColor = style.Colors[GuiColor::Header];
			windowColor = style.Colors[GuiColor::Window];
		}

		// Border
		s_Renderer->FillRect(borderPosition, borderSize, style.Colors[GuiColor::WindowBorder]);

		// Window body
		s_Renderer->FillRect(windowBodyPosition, windowBodySize, windowColor);

		// Window resize corner
		s_Renderer->FillRect(resizeCornerPosition, style.WindowResizeCornerSize, style.Colors[GuiColor::WindowResizeCorner]);
	}

}
