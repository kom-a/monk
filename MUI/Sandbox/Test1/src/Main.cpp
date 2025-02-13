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

void OnScroll(const mwl::MouseScrollEvent& e)
{
	mui::Input& input = mui::GetInput();

	input.Scroll.Y = e.Delta;
}

void OnKeyUp(const mwl::KeyUpEvent& e)
{

}

void OnMouseMove(const mwl::MouseMovedEvent& e)
{
	mui::Input& input = mui::GetInput();

	mouseX = e.X;
	mouseY = e.Y;

	input.MousePosition.X = mouseX;
	input.MousePosition.Y = mouseY;
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
	windowProps.VSync = true;
	windowProps.Width = 1600;
	windowProps.Height = 900;
	windowProps.Title = L"MUI";

	window = mwl::Create(windowProps);

	mwl::CursorData cursor("res/oxy-bluecurve/oxy-bluecurve.inf");
	window->LoadCursorData(cursor);

	window->SetWindowResizeCallback(OnWindowResize);
	window->SetKeyDownCallback(OnKeyDown);
	window->SetKeyUpCallback(OnKeyUp);
	window->SetMouseMovedCallback(OnMouseMove);
	window->SetMouseButtonDownCallback(OnButtonDown);
	window->SetMouseButtonUpCallback(OnButtonUp);
	window->SetMouseScrollCalback(OnScroll);

	mogl::OpenGLLoader::LoadOpenGL(mogl::OpenGLVersion::OPENGL_4_6);

	mui::InitForWin32(window->GetNative());

	mui::Input& input = mui::GetInput();

	uint32_t count = 0;

	bool flag = false;

	bool value1 = false;
	bool value2 = true;
	bool value3 = false;

	int radio_value = 0;
	int radio_value_2 = 0;

	while (!window->Closed())
	{
		window->Update();
		window->MakeContextCurrent();

		input.Viewport = mui::Vec2f(window->GetWidth(), window->GetHeight());

		glClearColor(0.85f, 0.75f, 0.75f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mui::NewFrame();

		for (size_t i = 0; i < 1; i++)
		{
			mui::Begin("Text wrap" + std::to_string(i));

			if (mui::Button("Click me"))
			{
				count++;
				flag = !flag;
			}

			mui::Text("count: " + std::to_string(count));

			if (flag)
			{
				mui::Separator();
				mui::Text("Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate", mui::TextFlags::TextFlags_WordWrap);
				mui::Separator();
			}

			if (mui::Button("Show demo window", mui::Vec2f(), mui::ButtonFlags_AutoFit))
				show_demo_window = true;

			mui::Separator();
			mui::Checkbox("Value 1", &value1);
			mui::Checkbox("Value 2", &value2);
			mui::Checkbox("Value 3", &value3);

			mui::Separator();

			mui::BeginRadio(&radio_value);
			mui::Radio("value 0", 0);
			mui::Radio("value 1", 1);
			mui::Radio("value 2", 2);
			mui::EndRadio();

			mui::Separator();

			mui::BeginRadio(&radio_value_2);
			mui::Radio("value 0", 0);
			mui::Radio("value 1", 1);
			mui::EndRadio();

			if (mui::Button("Helol"))
			{
				std::cout << "qweqwe" << std::endl;
			}

			mui::End();
		}

		mui::EndFrame();
		mui::Render();

		//if (mouseX > 500)
		//	window->SetCursor(mwl::CursorType::Hand);
		//else
		//	window->SetCursor(mwl::CursorType::Pointer);

		window->SwapBuffers();
	}

	mui::Shutdown();

	return 0;
}