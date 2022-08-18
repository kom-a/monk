#pragma once

#include "events/Event.h"

namespace monk
{
	class KeyEvent : public Event
	{
	public:
		int GetKeyCode() const { return m_KeyCode; }

	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {}

		virtual EventType GetEventType() const = 0;

		int m_KeyCode;
	};

	class KeyDownEvent : public KeyEvent
	{
	public:
		KeyDownEvent(int keycode, bool isRepeat = false)
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
		KeyUpEvent(int keycode)
			: KeyEvent(keycode) {}

		static EventType GetStaticType() { return EventType::KeyUp; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
	};
}