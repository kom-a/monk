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

	using WindowResizeCallbackFn	= std::function<void(WindowResizeEvent e)>;

	using MouseMovedCallbackFn		= std::function<void(MouseMovedEvent e)>;
	using MouseButtonDownCallbackFn = std::function<void(MouseButtonDownEvent e)>;
	using MouseButtonUpCallbackFn	= std::function<void(MouseButtonUpEvent e)>;
	using MouseClickedCallbackFn	= std::function<void(MouseClickedEvent e)>;
	using MouseScrollCallbackFn		= std::function<void(MouseScrollEvent e)>;

	using KeyDownCallbackFn			= std::function<void(KeyDownEvent e)>;
	using KeyUpCallbackFn			= std::function<void(KeyUpEvent e)>;

}