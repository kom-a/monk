#include <MWL/MWL.h>

#include <memory>
#include <iostream>

int main()
{
	std::shared_ptr<mwl::Window> window(mwl::Create());

	while (!window->Closed())
	{
		std::cout << "Width: " << window->GetWidth() << std::endl;
		std::cout << "Height: " << window->GetHeight() << std::endl;
		std::cout << std::endl;

		window->Update();
	}

	return 0;
}