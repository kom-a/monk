#include <iostream>

#include <WangMauna/WangMauna.h>
#include <MWL/MWL.h>
#include <MOGL/MOGL.h>
#include <MUI/MUI.h>

DEFINE_LOGGER();
IMPL_LOGGER("Mui Sandbox");

mwl::Window* window;

int mouseX = 0;
int mouseY = 0;
bool mouseDown = false;
bool show_demo_window = false;

void OnWindowResize(const mwl::WindowResizeEvent& e)
{
	glViewport(0, 0, e.Width, e.Height);
}

void OnKeyDown(const mwl::KeyDownEvent& e)
{
	if (e.Key == mwl::KeyCode::F11)
		window->SetFullscreen(!window->IsFullscreen());

	show_demo_window = true;
}

void OnKeyUp(const mwl::KeyUpEvent& e)
{
	
}

void OnMouseMove(const mwl::MouseMovedEvent& e)
{
	mui::Input& input = mui::GetInput();

	mouseX = e.X;
	mouseY = e.Y;

	input.MouseX = mouseX;
	input.MouseY = mouseY;
}

void OnButtonDown(const mwl::MouseButtonDownEvent& e)
{
	mouseDown = true;
	mui::Input& input = mui::GetInput();
	input.MouseLeftDown = true;
}

void OnButtonUp(const mwl::MouseButtonUpEvent& e)
{
	mouseDown = false;
	mui::Input& input = mui::GetInput();
	input.MouseLeftDown = false;
}

int main()
{
	mwl::WindowProps windowProps;
	windowProps.VSync = false;
	windowProps.Width = 1600;
	windowProps.Height = 900;
	windowProps.Title = L"MUI";

	window = mwl::Create(windowProps);

	window->SetWindowResizeCallback(OnWindowResize);
	window->SetKeyDownCallback(OnKeyDown);
	window->SetKeyUpCallback(OnKeyUp);
	window->SetMouseMovedCallback(OnMouseMove);
	window->SetMouseButtonDownCallback(OnButtonDown);
	window->SetMouseButtonUpCallback(OnButtonUp);

	mogl::OpenGLLoader::LoadOpenGL(mogl::OpenGLVersion::OPENGL_4_6);

	mui::InitForWin32((HWND)window->GetNative());
	
	while (!window->Closed())
	{
		window->Update();
		window->MakeContextCurrent();

		//glClearColor((float)mouseX / window->GetWidth(), (float)mouseY / window->GetHeight(), 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		mui::NewFrame();

		mui::Begin("Demo", &show_demo_window);
		mui::End();
		
		mui::Begin("Demo1");
		if (mui::Button("Open demo window"))
		{
			show_demo_window = true;
		}
		mui::End();

		mui::Begin("Demo2");
		mui::End();

		
		mui::EndFrame();
		mui::Render();

		window->SwapBuffers();
	}


	return 0;
}