#pragma once

#include "events/Event.h"
#include "input/KeyCodes.h"

namespace monk
{
	class KeyEvent : public Event
	{
	public:
		KeyCode GetKeyCode() const { return m_KeyCode; }

	protected:
		KeyEvent(KeyCode keycode)
			: m_KeyCode(keycode) {}

		virtual EventType GetEventType() const = 0;

		KeyCode m_KeyCode;
	};

	class KeyDownEvent : public KeyEvent
	{
	public:
		KeyDownEvent(KeyCode keycode, bool isRepeat = false)
			: KeyEvent(keycode), m_IsRepeat(isRepeat) {}

		bool IsRepeat() const { return m_IsRepeat; }

		static EventType GetStaticType() { return EventType::KeyDown; }
		virtual EventType GetEventType() const override { return GetStaticType(); }

	private:
		bool m_IsRepeat;
	};

	class KeyUpEvent : public KeyEvent
	{
	public:
		KeyUpEvent(KeyCode keycode)
			: KeyEvent(keycode) {}

		static EventType GetStaticType() { return EventType::KeyUp; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
	};
}