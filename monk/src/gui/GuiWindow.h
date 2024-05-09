#pragma once

#include <string>

#include <MMath/MMath.h>

namespace monk
{
	struct GuiWindowCacheData
	{
		std::string DebugName;
		mmath::vec2 Position;
		mmath::vec2 Size;
		uint32_t ZOrder;
		bool Collapsed;
	};

	struct GuiRect
	{
		mmath::vec2 Position;
		mmath::vec2 Size;

		bool IsInside(const mmath::vec2 point) const
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
		mmath::vec2 Center;
		float Radius;

		bool IsInside(const mmath::vec2& point) const 
		{
			return (Center - point).length2() <= Radius * 2;
		}
	};

	struct GuiWindow
	{
		std::string Name;
		uint32_t ID;

		mmath::vec2 Position;
		mmath::vec2 Size;
		mmath::vec2 CursorPosition;
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