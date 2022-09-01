#pragma once

#include <string>

#include "math/Math.h"

namespace monk
{
	struct GuiWindowCacheData
	{
		math::vec2 Position;
		math::vec2 Size;
	};

	struct GuiWindow
	{
		std::string Name;
		uint32_t ID;

		math::vec2 Position;
		math::vec2 Size;
		math::vec2 CursorPosition;

		GuiWindow* Next;
		GuiWindow* Prev;

		GuiWindow(const std::string& name);

		bool IsHot() const;
		bool IsResizeHot() const;

		GuiWindowCacheData GetCacheData() const;
	};
}