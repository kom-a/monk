#include "gui/Gui.h"

#include "input/Input.h"
#include "core/Log.h"

#include <functional>

// TODO: define this properely later
#define MONK_ASSERT(condition, message)

namespace monk
{
	static Renderer2D* Renderer;
	GuiStyle Gui::s_GuiStyle;
	std::unordered_map<uint32_t, GuiWindowCacheData> Gui::s_WindowCache;

	struct
	{
		GuiWindow* HeadWindow;
		GuiWindow* TailWindow;

		GuiWindow* CurrentWindow;

		uint32_t HotWindowID;
		uint32_t ActiveWindowID;

		uint32_t MovingWindowID;
		uint32_t ResizingWindowID;

		math::vec2 LastMouse;

		void AddWindowToList(GuiWindow* window)
		{
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
		}

	} GuiState;

	void Gui::Init()
	{
		Renderer = new Renderer2D();

		GuiStyle& style = Gui::GetStyle();

		GuiState.HeadWindow = nullptr;
		GuiState.TailWindow = nullptr;
		GuiState.CurrentWindow = nullptr;
		GuiState.ActiveWindowID = 0;
		GuiState.HotWindowID = 0;
		GuiState.MovingWindowID = 0;
		GuiState.ResizingWindowID = 0;
		GuiState.LastMouse = Input::GetMousePosition();

		style.Padding = math::vec2(5.0f, 5.0f);
		style.HeaderHeight = 16.0f;
		style.HeaderColor = math::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		style.HotHeaderColor = math::vec4(0.3f, 0.3f, 0.3f, 1.0f);
		style.ActiveHeaderColor = math::vec4(0.3f, 0.3f, 0.3f, 1.0f);
		style.WindowBackgroundColor = math::vec4(0.3f, 0.3f, 0.3f, 1.0f);
		style.HotWindowBackgroundColor = math::vec4(0.35f, 0.35f, 0.35f, 1.0f);
		style.ActiveWindowBackgroundColor = math::vec4(0.3f, 0.3f, 0.4f, 1.0f);
		style.WindowBorderSize = math::vec2(3.0f);
		style.WindowBorderColor = math::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		style.ButtonColor = math::vec4(0.2f, 0.2f, 0.6f, 1.0f);
		style.HotButtonColor = math::vec4(0.3f, 0.3f, 0.6f, 1.0f);
		style.ActiveButtonColor = math::vec4(0.1f, 0.1f, 0.4f, 1.0f);
		style.WindowResizeCornerSize = math::vec2(14.0f);
		style.WindowResizeCornerColor = math::vec4(0.1f, 0.2f, 0.3f, 1.0f);
		style.HotWindowResizeCornerColor = math::vec4(0.3f, 0.7f, 0.2f, 1.0f);
		style.ActiveWindowResizeCornerColor = math::vec4(0.2f, 0.4f, 0.1f, 1.0f);
	}

	GuiStyle& Gui::GetStyle()
	{
		return s_GuiStyle;
	}

	void Gui::NewFrame(const math::mat4& projection)
	{
		GuiState.HotWindowID = 0;
		Renderer->Begin(projection);
	}

	void Gui::EndFrame()
	{
		const GuiWindow* current = GuiState.HeadWindow;

		while (current)
		{
			s_WindowCache[current->ID] = current->GetCacheData();

			DrawWindow(current);
			current = current->Next;
		}
		
		Renderer->End();
		GuiState.ClearWindowList();
		GuiState.HotWindowID = 0;
		GuiState.LastMouse = Input::GetMousePosition();
		
		if (!Input::IsMouseButtonPressed(Mouse::ButtonLeft))
		{
			GuiState.MovingWindowID = 0;
			GuiState.ResizingWindowID = 0;
		}
	}

	void Gui::Begin(const std::string& name)
	{
		GuiWindow* window = new GuiWindow(name);
		Gui::RestoreWindow(window);

		GuiState.AddWindowToList(window);
		GuiState.CurrentWindow = window;

		
		if (window->IsHot())
		{
			GuiState.HotWindowID = window->ID;
			if (Input::IsMouseButtonDown(Mouse::ButtonLeft))
			{
				GuiState.ActiveWindowID = window->ID;
				GuiState.MovingWindowID = window->ID;
				GuiState.ResizingWindowID = 0;
			}
		}

		if (window->IsResizeHot())
		{
			if (Input::IsMouseButtonDown(Mouse::ButtonLeft))
			{
				GuiState.ResizingWindowID = window->ID;
				GuiState.MovingWindowID = 0;
			}
		}
		
		if (window->ID == GuiState.ResizingWindowID)
		{
			window->Size += Input::GetMousePosition() - GuiState.LastMouse;
		}
		else if (window->ID == GuiState.MovingWindowID)
		{
			window->Position += Input::GetMousePosition() - GuiState.LastMouse;
		}
	}

	void Gui::End()
	{
		MONK_ASSERT(GuiState.CurrentWindow, "No GuiWindow to 'End' ");

		GuiState.CurrentWindow = nullptr;
	}

	void Gui::RestoreWindow(GuiWindow* window)
	{
		if (s_WindowCache.find(window->ID) != s_WindowCache.end())
		{
			const GuiWindowCacheData& cache = s_WindowCache.at(window->ID);

			window->Position = cache.Position;
			window->Size = cache.Size;
		}
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
		Renderer->DrawRect(borderPosition, borderSize, style.WindowBorderColor);

		// Header
		Renderer->DrawRect(headerPosition, headerSize, headerColor);

		// Window body
		Renderer->DrawRect(windowBodyPosition, windowBodySize, windowColor);

		// Window resize corner
		Renderer->DrawRect(resizeCornerPosition, style.WindowResizeCornerSize, style.WindowResizeCornerColor);
	}

}