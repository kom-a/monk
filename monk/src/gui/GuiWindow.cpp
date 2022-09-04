#include "gui/GuiWindow.h"

#include <functional>

#include "gui/Gui.h"
#include "input/Input.h"

namespace monk
{
	GuiWindow::GuiWindow(const std::string& name)
		:
		Name(name),
		Position(math::vec2(100, 100)),
		Size(math::vec2(150, 50)),
		ZOrder(0),
		Prev(nullptr),
		Next(nullptr)
	{
		std::hash<std::string> hasher;
		ID = static_cast<uint32_t>(hasher(Name));
	}

	bool GuiWindow::IsHot() const
	{
		float x = Input::GetMouseX();
		float y = Input::GetMouseY();

		const GuiStyle& style = Gui::GetStyle();

		return (x >= Position.x && x <= Position.x + Size.x) &&
			(y >= Position.y && y <= style.HeaderHeight + Position.y + Size.y);
	}

	bool GuiWindow::IsResizeHot() const
	{
		const GuiStyle& style = Gui::GetStyle();

		GuiRect resize = GetResizeCornerRect();

		return resize.IsInside(Input::GetMousePosition());
	}

	bool GuiWindow::IsCloseButtonHot() const
	{
		const GuiStyle& style = Gui::GetStyle();

		GuiRect closeRect = GetCloseButtonRect();
		return closeRect.IsInside(Input::GetMousePosition());
	}

	GuiWindowCacheData GuiWindow::GetCacheData() const
	{
		GuiWindowCacheData data = {
			Name,
			Position,
			Size,
			ZOrder
		};

		return data;
	}

	GuiRect GuiWindow::GetResizeCornerRect() const
	{
		const GuiStyle& style = Gui::GetStyle();

		math::vec2 windowBodyPosition = math::vec2(Position.x, Position.y + style.HeaderHeight);

		GuiRect resize;
		resize.Size = style.WindowResizeCornerSize;
		resize.Position = windowBodyPosition + Size - resize.Size;

		return resize;
	}

	GuiRect GuiWindow::GetCloseButtonRect() const
	{
		const GuiStyle& style = Gui::GetStyle();
		math::vec2 headerSize = math::vec2(Size.x, style.HeaderHeight);

		GuiRect close;
		close.Size = math::vec2(style.HeaderHeight * 0.8 * 2, style.HeaderHeight * 0.8);
		close.Position = Position + math::vec2(headerSize.x - close.Size.x - style.HeaderHeight * 0.1, style.HeaderHeight * 0.1);

		return close;
	}
}