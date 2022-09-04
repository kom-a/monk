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

		math::vec2 windowBodyPosition = Position + math::vec2(0.0f, style.HeaderHeight);
		math::vec2 resizeCornerPosition = windowBodyPosition + Size - style.WindowResizeCornerSize;

		float mouseX = Input::GetMouseX();
		float mouseY = Input::GetMouseY();

		float x1 = resizeCornerPosition.x;
		float x2 = resizeCornerPosition.x + style.WindowResizeCornerSize.x;
		float y1 = resizeCornerPosition.y;
		float y2 = resizeCornerPosition.y + style.WindowResizeCornerSize.y;

		return mouseX >= x1 && mouseX <= x2 && mouseY >= y1 && mouseY <= y2;
	}

	bool GuiWindow::IsCloseButtonHot() const
	{
		const GuiStyle& style = Gui::GetStyle();

		GuiRect closeRect = GetCloseButton();
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

	GuiRect GuiWindow::GetCloseButton() const
	{
		const GuiStyle& style = Gui::GetStyle();
		math::vec2 headerSize = math::vec2(Size.x, style.HeaderHeight);

		GuiRect close;
		close.Size = math::vec2(style.HeaderHeight * 0.8 * 2, style.HeaderHeight * 0.8);
		close.Position = Position + math::vec2(headerSize.x - close.Size.x - style.HeaderHeight * 0.1, style.HeaderHeight * 0.1);

		return close;
	}
}