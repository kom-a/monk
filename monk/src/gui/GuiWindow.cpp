#include "gui/GuiWindow.h"

#include <functional>

#include "gui/Gui.h"
#include "input/Input.h"

namespace monk
{
	GuiWindow::GuiWindow(const std::string& name, bool* open)
		:
		Name(name),
		Position(math::vec2(100, 100)),
		Size(math::vec2(150, 50)),
		ZOrder(0),
		Collapsed(false),
		Open(open),
		Prev(nullptr),
		Next(nullptr)
	{
		std::hash<std::string> hasher;
		ID = static_cast<uint32_t>(hasher(Name));
	}

	void GuiWindow::Restore(const GuiWindowCacheData& cacheData)
	{
		Position = cacheData.Position;
		Size = cacheData.Size;
		ZOrder = cacheData.ZOrder;
		Collapsed = cacheData.Collapsed;
	}

	bool GuiWindow::IsHot() const
	{
		const GuiStyle& style = Gui::GetStyle();

		GuiRect headerRect;
		headerRect.Position = Position;
		headerRect.Size = math::vec2(Size.x, style.HeaderHeight);

		GuiRect windowRect;
		windowRect.Position = Position;
		windowRect.Size = Size + math::vec2(0.0f, headerRect.Size.y);

		bool hot;
		if (Collapsed)
			hot = headerRect.IsInside(Input::GetMousePosition());
		else
			hot = windowRect.IsInside(Input::GetMousePosition());

		return hot;
	}

	bool GuiWindow::IsResizeHot() const
	{
		GuiRect resize = GetResizeCorner();

		return resize.IsInside(Input::GetMousePosition());
	}

	bool GuiWindow::IsCollapseHot() const
	{
		GuiRect collapse = GetCollapseButton();

		return collapse.IsInside(Input::GetMousePosition());
	}

	bool GuiWindow::IsCloseHot() const
	{
		const GuiStyle& style = Gui::GetStyle();

		GuiCircle closeRect = GetCloseButton();
		return closeRect.IsInside(Input::GetMousePosition());
	}

	GuiWindowCacheData GuiWindow::GetCacheData() const
	{
		GuiWindowCacheData data = {
			Name,
			Position,
			Size,
			ZOrder,
			Collapsed
		};

		return data;
	}

	GuiRect GuiWindow::GetResizeCorner() const
	{
		const GuiStyle& style = Gui::GetStyle();

		math::vec2 windowBodyPosition = math::vec2(Position.x, Position.y + style.HeaderHeight);

		GuiRect resize;
		resize.Size = style.WindowResizeCornerSize;
		resize.Position = windowBodyPosition + Size - resize.Size;

		return resize;
	}

	GuiRect GuiWindow::GetCollapseButton() const
	{
		const GuiStyle& style = Gui::GetStyle();
		math::vec2 headerSize = math::vec2(Size.x, style.HeaderHeight);
		const float buttonHeightPercentOfHeaderHeight = 0.6; // close button occupy 60% of headerHeight
		const float& percent = buttonHeightPercentOfHeaderHeight;

		GuiRect collapse;
		collapse.Size = math::vec2(style.HeaderHeight * percent);
		collapse.Position = Position + math::vec2(style.HeaderPadding, style.HeaderHeight * (1.0f - percent) / 2);

		return collapse;
	}

	GuiCircle GuiWindow::GetCloseButton() const
	{
		const GuiStyle& style = Gui::GetStyle();
		math::vec2 headerSize = math::vec2(Size.x, style.HeaderHeight);
		const float buttonHeightPercentOfHeaderHeight = 0.8; // close button occupy 80% of headerHeight
		const float& percent = buttonHeightPercentOfHeaderHeight;

		GuiCircle close;
		close.Radius = style.HeaderHeight * percent / 2.0f;
		
		close.Center.x = Position.x + Size.x - style.HeaderPadding - close.Radius;
		close.Center.y = Position.y + style.HeaderHeight / 2.0f;

		return close;
	}
}