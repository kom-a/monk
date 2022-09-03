#pragma once

#include <string>

#include "math/Math.h"

namespace monk
{
	struct GuiWindowCacheData
	{
		std::string DebugName;
		math::vec2 Position;
		math::vec2 Size;
		uint32_t ZOrder;
	};

	struct GuiWindow
	{
		std::string Name;
		uint32_t ID;

		math::vec2 Position;
		math::vec2 Size;
		math::vec2 CursorPosition;
		uint32_t ZOrder;

		GuiWindow* Next;
		GuiWindow* Prev;

		GuiWindow(const std::string& name);

		bool IsHot() const;
		bool IsResizeHot() const;

		GuiWindowCacheData GetCacheData() const;
	};
}