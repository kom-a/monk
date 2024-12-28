#pragma once

#include <string>

#include <MML/MML.h>

namespace monk
{
	struct GuiWindowCacheData
	{
		std::string DebugName;
		mml::vec2 Position;
		mml::vec2 Size;
		uint32_t ZOrder;
		bool Collapsed;
	};

	struct GuiRect
	{
		mml::vec2 Position;
		mml::vec2 Size;

		bool IsInside(const mml::vec2 point) const
		{
			const float& x = point.x;
			const float& y = point.y;

			float x1 = Position.x;
			float x2 = Position.x + Size.x;
			float y1 = Position.y;
			float y2 = Position.y + Size.y;

			return (x >= x1 && x <= x2) && (y >= y1 && y <= y2);
		}
	};

	struct GuiCircle
	{
		mml::vec2 Center;
		float Radius;

		bool IsInside(const mml::vec2& point) const 
		{
			return (Center - point).length2() <= Radius * 2;
		}
	};

	struct GuiWindow
	{
		std::string Name;
		uint32_t ID;

		mml::vec2 Position;
		mml::vec2 Size;
		mml::vec2 CursorPosition;
		uint32_t ZOrder;
		bool Collapsed;
		bool* Open;

		GuiWindow* Next;
		GuiWindow* Prev;

		GuiWindow(const std::string& name, bool* open);
		
		void Restore(const GuiWindowCacheData& cacheData);
		
		bool IsHot() const;
		bool IsResizeHot() const;
		bool IsCollapseHot() const;
		bool IsCloseHot() const;

		GuiWindowCacheData GetCacheData() const;
		GuiRect GetResizeCorner() const;
		GuiRect GetCollapseButton() const;
		GuiRect GetCloseButton() const;
	};
}