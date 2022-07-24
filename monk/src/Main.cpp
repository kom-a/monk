#include <iostream>

#include "core/Window.h"
#include "core/Log.h"

int main()
{
	using namespace monk;

	Log::GetInstance().SetLevel(Log::Level::TRACE);

	Window window(1280, 720, "Monk");

	while (true)
	{
		window.Update();
	}

	return 0;
}