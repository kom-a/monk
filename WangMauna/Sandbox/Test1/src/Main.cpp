#include <iostream>

#include <type_traits>

#include <WangMauna/WangMauna.h>

int foo()
{
	return 52;
}

int main()
{
	auto logger = wm::Logger("WangMauna", "%n| [%t] %l: %v");

	double pi = 3.1415;
	const std::wstring world = L"world";
	
	logger.Trace("{1}", 123, pi, 123);
	logger.Trace("test {0}, {1} привет", L"Arg1", foo());
	logger.Trace("This is a trace {0}, {1", L"Arg1", foo());
	logger.Info("Hello world this is an info message {1}", 123, pi);
	logger.Debug("{0}{0}{1}", "hello", world);

	logger.Info("Logger info");
	logger.Debug("This is a debug information");
	logger.Warning("Warning! Warning! Warning! Warning!");
	logger.Error("Error");
	logger.Critical("Critical error.");

	return 0;
}