#pragma once

namespace monk
{
	enum class EventType
	{
		None = 0,
		KeyDown, KeyUp,
		MouseButtonDown, MouseButtonUp, MouseMove, MouseScroll,
	};

	class Event
	{
	public:
		virtual ~Event() = default;

		bool Handled = false;

		virtual EventType GetEventType() const = 0;

		template<typename T, typename F>
		void Dispatch(const F& func)
		{
			if (GetEventType() == T::GetStaticType() && !Handled)
				Handled = func(static_cast<T&>(*this));
		}
	};
}