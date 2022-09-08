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
		GuiRect resize = GetResizeCornerRect();

		return resize.IsInside(Input::GetMousePosition());
	}

	bool GuiWindow::IsCollapseHot() const
	{
		GuiRect collapse = GetCollapseButtonRect();

		return collapse.IsInside(Input::GetMousePosition());
	}

	bool GuiWindow::IsCloseHot() const
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
			ZOrder,
			Collapsed
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

	GuiRect GuiWindow::GetCollapseButtonRect() const
	{
		const GuiStyle& style = Gui::GetStyle();
		
		GuiRect closeButtonRect = GetCloseButtonRect();
		GuiRect collapse;
		collapse.Size = closeButtonRect.Size;
		collapse.Position = closeButtonRect.Position;
		collapse.Position.x -= collapse.Size.x + style.HeaderPadding;

		return collapse;
	}

	GuiRect GuiWindow::GetCloseButtonRect() const
	{
		const GuiStyle& style = Gui::GetStyle();
		math::vec2 headerSize = math::vec2(Size.x, style.HeaderHeight);
		const float closeButtonHeightPercentOfHeaderHeight = 0.6; // close button occupy 60% of headerHeight
		const float& percent = closeButtonHeightPercentOfHeaderHeight;

		GuiRect close;
		close.Size = math::vec2(style.HeaderHeight * percent * 2, style.HeaderHeight * percent);
		close.Position = Position + math::vec2(headerSize.x - close.Size.x - style.HeaderHeight * (1.0f - percent) / 2, style.HeaderHeight * (1.0f - percent) / 2);

		return close;
	}
}