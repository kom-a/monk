#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"

namespace mwl
{
	struct WindowResizeEvent
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
	};

	struct MouseMovedEvent
	{
		uint32_t X = 0;
		uint32_t Y = 0;
	};

	struct MouseButtonDownEvent
	{
		uint32_t X = 0;
		uint32_t Y = 0;
		MouseButton Button = MouseButton::None;
	};

	struct MouseButtonUpEvent
	{
		uint32_t X = 0;
		uint32_t Y = 0;
		MouseButton Button = MouseButton::None;
	};

	struct MouseClickedEvent
	{
		uint32_t X = 0;
		uint32_t Y = 0;
		MouseButton Button = MouseButton::None;
	};

	struct MouseScrollEvent
	{
		int32_t Delta = 0;
	};

	struct KeyDownEvent
	{
		KeyCode Key = KeyCode::None;
		bool Repeat = false;
	};

	struct KeyUpEvent
	{
		KeyCode Key = KeyCode::None;
	};

	using WindowResizeDelegate = Event<WindowResizeEvent>;

	using MouseMovedDelegate = Event<MouseMovedEvent>;
	using MouseButtonDownDelegate = Event<MouseButtonDownEvent>;
	using MouseButtonUpDelegate = Event<MouseButtonUpEvent>;
	using MouseClickedDelegate = Event<MouseClickedEvent>;
	using MouseScrollDelegate = Event<MouseScrollEvent>;

	using KeyDownDelegate = Event<KeyDownEvent>;
	using KeyUpDelegate = Event<KeyUpEvent>;
} // namespace mwl