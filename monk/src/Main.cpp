#include <iostream>

#include "core/Window.h"
#include "core/Log.h"

int main()
{
	using namespace monk;

	Window window(1280, 720, "Monk");

	while (!window.Closed())
	{
		window.Update();
	}

	return 0;
}
