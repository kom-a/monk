#include "MUI.h"

#include <cassert>

#include "Renderer.h"

namespace mui
{

	Window::Window(const std::string& name)
		: Name(name)
	{ }

	Viewport::Viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		: X(x), Y(y), Width(width), Height(height)
	{ }
	
	std::list<Window> Mui::s_Windows;
	Mui::State Mui::s_State;
	
	std::unique_ptr<Renderer> Mui::s_Renderer;
	Viewport Mui::s_Viewport;

	void Mui::Init()
	{
		s_Renderer = std::make_unique<Renderer>();
	}

	void Mui::ShutDown()
	{
		s_Renderer.reset();
	}

	void Mui::NewFrame(const Viewport& viewport)
	{
		s_Viewport = viewport;
	}

	void Mui::EndFrame()
	{ 

	}

	void Mui::Begin(const std::string& name)
	{
		assert(s_State.CurrentWindow == nullptr && "Can not 'Begin' new window inside another one. Call 'End' before starting new window.");

		s_Windows.emplace_back(name);
		s_State.CurrentWindow = &s_Windows.back();
	}

	void Mui::End()
	{
		assert(s_State.CurrentWindow != nullptr && "No mui::Window to 'End'");

		s_State.CurrentWindow = nullptr;
	}

	bool Mui::Button(std::string_view text)
	{
		return false;
	}
}

