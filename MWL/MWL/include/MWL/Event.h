#pragma once

#include <functional>

namespace mwl
{
	template<typename... Args>
	class Event
	{
	public:
		using Callback = std::function<void(Args...)>;

	public:
		void operator+=(Callback cb)
		{
			m_Callbacks.push_back(cb);
		}

		void operator-=(Callback cb)
		{
			m_Callbacks.erase(
				std::remove(m_Callbacks.begin(), m_Callbacks.end(), cb),
				m_Callbacks.end()
			);
		}

		void Invoke(Args... args) const
		{
			for (const auto& cb : m_Callbacks)
			{
				cb(args...);
			}
		}

	private:
		std::vector<Callback> m_Callbacks;
	};
}
