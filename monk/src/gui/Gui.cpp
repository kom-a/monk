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

		GuiStyle& style = Gui::GetStyle();
		style = DefaultStyle();

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

	void Gui::Begin(const std::string& name)
	{
		MONK_ASSERT(!GuiState.CurrentWindow, "Can not 'Begin' new window inside another one. Call 'End' before starting new window.");

		GuiWindow* window = new GuiWindow(name);
		Gui::RestoreWindow(window);

		Gui::BeginWindow(window);

		GuiState.AddWindowToList(window);
		GuiState.CurrentWindow = window;
	}

	void Gui::End()
	{
		MONK_ASSERT(GuiState.CurrentWindow, "No GuiWindow to 'End'");

		GuiState.CurrentWindow = nullptr;
	}

	//////////////////////////////////////////////////////////////////////////
	// Private
	//////////////////////////////////////////////////////////////////////////

	GuiStyle Gui::DefaultStyle()
	{
		GuiStyle style;

		style.Padding = math::vec2(5.0f, 5.0f);
		style.HeaderHeight = 16.0f;
		style.HeaderColor = math::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		style.HotHeaderColor = math::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		style.ActiveHeaderColor = math::vec4(0.1f, 0.1f, 0.2f, 1.0f);
		style.MinWindowSize = math::vec2(100, 25);
		style.WindowBackgroundColor = math::vec4(0.3f, 0.3f, 0.3f, 1.0f);
		style.HotWindowBackgroundColor = math::vec4(0.35f, 0.35f, 0.35f, 1.0f);
		style.ActiveWindowBackgroundColor = math::vec4(0.3f, 0.3f, 0.3f, 1.0f);
		style.WindowBorderSize = math::vec2(3.0f);
		style.WindowBorderColor = math::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		style.ButtonColor = math::vec4(0.2f, 0.2f, 0.6f, 1.0f);
		style.HotButtonColor = math::vec4(0.3f, 0.3f, 0.6f, 1.0f);
		style.ActiveButtonColor = math::vec4(0.1f, 0.1f, 0.4f, 1.0f);
		style.WindowResizeCornerSize = math::vec2(14.0f);
		style.WindowResizeCornerColor = math::vec4(0.1f, 0.2f, 0.3f, 1.0f);
		style.HotWindowResizeCornerColor = math::vec4(0.3f, 0.7f, 0.2f, 1.0f);
		style.ActiveWindowResizeCornerColor = math::vec4(0.2f, 0.4f, 0.1f, 1.0f);

		return style;
	}

	void Gui::RestoreWindow(GuiWindow* window)
	{
		if (s_WindowCache.find(window->ID) != s_WindowCache.end())
		{
			const GuiWindowCacheData& cache = s_WindowCache.at(window->ID);

			window->Position = cache.Position;
			window->Size = cache.Size;
			window->ZOrder = cache.ZOrder;
		}
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
					GuiState.ResizingWindowID = window->ID;
				else
					GuiState.MovingWindowID = window->ID;
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
		GuiStyle& style = Gui::GetStyle();

		const math::vec2& headerPosition = window->Position;
		math::vec2 headerSize = math::vec2(window->Size.x, style.HeaderHeight);
		math::vec2 windowBodyPosition = math::vec2(window->Position.x, window->Position.y + style.HeaderHeight);
		const math::vec2& windowBodySize = window->Size;
		math::vec2 borderPosition = window->Position - style.WindowBorderSize;
		math::vec2 borderSize = window->Size + math::vec2(0, style.HeaderHeight) + style.WindowBorderSize * 2;
		math::vec2 resizeCornerPosition = windowBodyPosition + windowBodySize - style.WindowResizeCornerSize;
		math::vec2 closeButtonSize = math::vec2(style.HeaderHeight * 0.8 * 2, style.HeaderHeight * 0.8);
		math::vec2 closeButtonPosition = headerPosition + math::vec2(headerSize.x - closeButtonSize.x - style.HeaderHeight * 0.1, style.HeaderHeight * 0.1);

		math::vec4 headerColor;
		math::vec4 windowColor;

		if (window->ID == GuiState.ActiveWindowID)
		{
			headerColor = style.ActiveHeaderColor;
			windowColor = style.ActiveWindowBackgroundColor;
		}
		else if (window->ID == GuiState.HotWindowID)
		{
			headerColor = style.HotHeaderColor;
			windowColor = style.HotWindowBackgroundColor;
		}
		else
		{
			headerColor = style.HeaderColor;
			windowColor = style.WindowBackgroundColor;
		}

		// Border
		s_Renderer->DrawRect(borderPosition, borderSize, style.WindowBorderColor);

		// Header
		s_Renderer->DrawRect(headerPosition, headerSize, headerColor);

		// Window body
		s_Renderer->DrawRect(windowBodyPosition, windowBodySize, windowColor);

		// Window resize corner
		s_Renderer->DrawRect(resizeCornerPosition, style.WindowResizeCornerSize, style.WindowResizeCornerColor);

		// Window close button
		s_Renderer->DrawRect(closeButtonPosition, closeButtonSize, math::vec4(0.8f, 0.4f, 0.4f, 1.0f));
	}

}