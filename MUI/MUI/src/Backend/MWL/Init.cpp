#include <MWL/MWL.h>
#include <MUI/MUI.h>

namespace mui
{
	void OnMouseMove(const mwl::MouseMovedEvent& e)
	{
		mui::Input& input = mui::GetInput();
		input.MousePosition = { float(e.X), float(e.Y) };
	}

	void OnMouseButtonDown(const mwl::MouseButtonDownEvent& e)
	{
		mui::Input& input = mui::GetInput();
		input.MouseLeftDown = e.Button == mwl::MouseButton::Left;
	}

	void OnMouseButtonUp(const mwl::MouseButtonUpEvent& e)
	{
		mui::Input& input = mui::GetInput();
		input.MouseLeftDown = !(e.Button == mwl::MouseButton::Left);
	}

	void OnViewportResize(const mwl::WindowResizeEvent& e)
	{
		mui::Input& input = mui::GetInput();
		input.Viewport = { (float)e.Width, (float)e.Height };
	}

	void InitForMWLImpl(void* MWLWindow)
	{
		mwl::Window* window = static_cast<mwl::Window*>(MWLWindow);

		window->MouseMoved() += OnMouseMove;
		window->MouseButtonDown() += OnMouseButtonDown;
		window->MouseButtonUp() += OnMouseButtonUp;
		window->Resized() += OnViewportResize;

		mui::Input& input = mui::GetInput();
		input.Viewport = { (float)window->GetWidth(), (float)window->GetHeight() };
	}
}
