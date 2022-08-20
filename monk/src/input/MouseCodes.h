#pragma once

#include <inttypes.h>

namespace monk
{
	using MouseCode = uint16_t;

	namespace Mouse
	{
		enum : MouseCode
		{
			Button0 = 0x01,
			Button1 = 0x02,
			Button2 = 0x04,

			ButtonLeft = Button0,
			ButtonRight = Button1,
			ButtonMiddle = Button2
		};
	}
}