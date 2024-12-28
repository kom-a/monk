#include "gui/GuiWindow.h"

#include <functional>

#include "gui/Gui.h"
#include "input/Input.h"

namespace monk
{
	GuiWindow::GuiWindow(const std::string& name, bool* open)
		:
		Name(name),
		Position(mml::vec2(100, 100)),
		Size(mml::vec2(150, 50)),
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
		headerRect.Size = mml::vec2(Size.x, style.HeaderHeight);

		GuiRect windowRect;
		windowRect.Position = Position;
		windowRect.Size = Size + mml::vec2(0.0f, headerRect.Size.y);

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

		GuiRect closeRect = GetCloseButton();
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

		mml::vec2 windowBodyPosition = mml::vec2(Position.x, Position.y + style.HeaderHeight);

		GuiRect resize;
		resize.Size = style.WindowResizeCornerSize;
		resize.Position = windowBodyPosition + Size - resize.Size;

		return resize;
	}

	GuiRect GuiWindow::GetCollapseButton() const
	{
		const GuiStyle& style = Gui::GetStyle();
		mml::vec2 headerSize = mml::vec2(Size.x, style.HeaderHeight);
		const float buttonHeightPercentOfHeaderHeight = 0.6; // close button occupy 60% of headerHeight
		const float& percent = buttonHeightPercentOfHeaderHeight;

		GuiRect collapse;
		collapse.Size = mml::vec2(style.HeaderHeight * percent);
		collapse.Position = Position + mml::vec2(style.HeaderPadding, style.HeaderHeight * (1.0f - percent) / 2);

		return collapse;
	}

	GuiRect GuiWindow::GetCloseButton() const
	{
		const GuiStyle& style = Gui::GetStyle();
		mml::vec2 headerSize = mml::vec2(Size.x, style.HeaderHeight);
		const float buttonHeightPercentOfHeaderHeight = 0.8f; // close button occupy 80% of headerHeight
		const float& percent = buttonHeightPercentOfHeaderHeight;

		const float closeButtonWidth = 30;

		GuiRect close;
		close.Size = mml::vec2(closeButtonWidth, headerSize.y * percent);
		close.Position = Position + mml::vec2(Size.x - close.Size.x - style.HeaderPadding, headerSize.y * (1.0f - percent) * 0.5f);
		
		return close;
	}
}