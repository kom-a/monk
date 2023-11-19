#include <iostream>

#include <WangMauna/WangMauna.h>

int main()
{
	std::cout << "Hello world" << std::endl;

	//pattern.Push(wm::Attribute::String, "WangMuana");
	//pattern.Push(wm::Attribute::String, "|");
	//pattern.Push(wm::Attribute::Time);
	//pattern.Push(wm::Attribute::Level);
	//pattern.Push(wm::Attribute::Value);
	//pattern.Push(wm::Attribute::String, "(");
	//pattern.Push(wm::Attribute::Filepath);
	//pattern.Push(wm::Attribute::Filename);
	//pattern.Push(wm::Attribute::String, ":");
	//pattern.Push(wm::Attribute::Line);
	//pattern.Push(wm::Attribute::String, ")");

	wm::Logger logger = wm::Logger();
	logger.SetName("WangMauna");
	logger.SetPattern("%n| [%t] %l: %v (%fn:%fl)");

	logger.Trace("Hello world. This is a trace");
	logger.Info("Logger info");
	logger.Debug("This is a debug information");
	logger.Warn("Warning! Warning! Warning! Warning! ");
	logger.Error("Error");
	logger.Critical("Legit check");

	return 0;
}