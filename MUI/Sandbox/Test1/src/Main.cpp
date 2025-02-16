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
bool show_gui = false;

void OnWindowResize(const mwl::WindowResizeEvent& e)
{
	glViewport(0, 0, e.Width, e.Height);
}

void OnKeyDown(const mwl::KeyDownEvent& e)
{
	if (e.Key == mwl::KeyCode::F11)
		window->SetFullscreen(!window->IsFullscreen());
	if (e.Key == mwl::KeyCode::Space)
		show_gui = !show_gui;

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

	int slider_int1 = -1;
	int slider_int2 = -1;
	int slider_int3 = -1;

	bool show_sliders = false;
	bool show_drags = false;

	int slider2[2] = { 0, 0 };
	int slider3[3] = { 0, 0, 0 };
	int slider4[4] = { 0, 0, 0, 0 };

	float slider_float = 0.0f;
	float slider_float2[2] = { 0.0f, 0.0f };
	float slider_float3[3] = { 0.0f, 0.0f, 0.0f};
	float slider_float4[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	int drag_int_0_100 = 0;
	int drag_int = 0;
	int drag2[2] = { 0, 0 };
	int drag3[3] = { 0, 0, 0 };
	int drag4[4] = { 0, 0, 0, 0 };

	float drag_float_0_100 = 0;
	float drag_float = 0;
	float drag2_float[2] = { 0, 0 };
	float drag3_float[3] = { 0, 0, 0 };
	float drag4_float[4] = { 0, 0, 0, 0 };

	float clear_color[3] = { 0.0f, 0.0f, 0.0f };

	while (!window->Closed())
	{
		window->Update();
		window->MakeContextCurrent();

		input.Viewport = mui::Vec2f(window->GetWidth(), window->GetHeight());

		glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (show_gui)
		{
			mui::NewFrame();

			for (size_t i = 0; i < 1; i++)
			{
				mui::Begin("Text wrap" + std::to_string(i));

				if (mui::Button("Click me++"))
				{
					slider4[0]++;
				}
				if (mui::Button("Click me--"))
				{
					slider4[0]--;
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

				if (mui::Button("Show sliders", mui::Vec2f(), mui::ButtonFlags_AutoFit))
					show_sliders = true;

				if (mui::Button("Show drags", mui::Vec2f(), mui::ButtonFlags_AutoFit))
					show_drags = true;


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

			mui::Begin("Sliders", &show_sliders);

			mui::SliderInt("slider int", &slider_int1, -2, 1);
			mui::SliderInt2("slider int 2", slider2, 0, 10);
			mui::SliderInt3("slider int 3", slider3, 0, 255);
			mui::SliderInt4("slider int 4", slider4, -2, 1);

			mui::Separator();

			mui::SliderFloat("slider float", &slider_float, 0.25f, -1.0f, 1.0f);
			mui::SliderFloat2("slider float 2", slider_float2, 0.2f, -2.0f, 1.0f);
			mui::SliderFloat3("slider float 3", slider_float3, 0.2f, -2.0f, 1.0f);
			mui::SliderFloat4("slider float 4", slider_float4, 0.02f, -2.0f, 1.0f);

			mui::SliderFloat3("Clear color", clear_color, 0.01f, 0.0f, 1.0f);

			mui::End();


			mui::Begin("Drags", &show_drags);

			mui::DragInt("drag int 0 100", &drag_int_0_100, 1, 0, 100);
			mui::DragInt("drag int", &drag_int);
			mui::DragInt2("drag int 2", drag2);
			mui::DragInt3("drag int 3", drag3);
			mui::DragInt4("drag int 4", drag4);

			mui::Separator();

			mui::DragFloat("drag float 0 100", &drag_float_0_100, 1, 0, 100);
			mui::DragFloat("drag float", &drag_float);
			mui::DragFloat2("drag float 2", drag2_float);
			mui::DragFloat3("drag float 3", drag3_float);
			mui::DragFloat4("drag float 4", drag4_float);

			mui::End();

			mui::EndFrame();
			mui::Render();
		}

		//if (mouseX > 500)
		//	window->SetCursor(mwl::CursorType::Hand);
		//else
		//	window->SetCursor(mwl::CursorType::Pointer);

		window->SwapBuffers();
	}

	mui::Shutdown();

	return 0;
}