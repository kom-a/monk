#include <iostream>

#include <Scriptorium/Scriptorium.h>

int main()
{
	using namespace Scriptorium;

	uint8_t* buffer = new uint8_t[1 << 20];
	fread(buffer, 1, 1 << 20, fopen("C:/Windows/Fonts/Carlito-Regular.ttf", "rb"));

	Reader reader(buffer);

	const std::byte* bytes = reader.ReadBuffer(15);
	const uint8_t* qweqwe = (uint8_t*)bytes;
	
	std::cout << "hello world " << reader.ReadUInt32() << std::endl;
	return 0;
}