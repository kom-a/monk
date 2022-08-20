#pragma once

#include "events/Event.h"
#include "input/MouseCodes.h"

namespace monk
{
	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float x, float y)
			: m_MouseX(x), m_MouseY(y) {}

		float GetX() const { return m_MouseX; }
		float GetY() const { return m_MouseY; }

		static EventType GetStaticType() { return EventType::MouseMove; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		
	private:
		float m_MouseX, m_MouseY;
	};

	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {}

		float GetXOffset() const { return m_XOffset; }
		float GetYOffset() const { return m_YOffset; }

		static EventType GetStaticType() { return EventType::MouseScroll; }
		virtual EventType GetEventType() const override { return GetStaticType(); }

	private:
		float m_XOffset, m_YOffset;
	};

	class MouseButtonDownEvent : public Event
	{
	public:
		MouseButtonDownEvent(MouseCode button)
			: m_Button(button) {}

		MouseCode GetButton() const { return m_Button; }

		static EventType GetStaticType() { return EventType::MouseButtonDown; }
		virtual EventType GetEventType() const override { return GetStaticType(); }

	protected:
		MouseCode m_Button;
	};

	class MouseButtonUpEvent : public Event
	{
	public:
		MouseButtonUpEvent(MouseCode button)
			: m_Button(button) {}

		MouseCode GetButton() const { return m_Button; }

		static EventType GetStaticType() { return EventType::MouseButtonUp; }
		virtual EventType GetEventType() const override { return GetStaticType(); }

	protected:
		MouseCode m_Button;
	};

}