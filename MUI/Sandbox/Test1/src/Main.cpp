#include <iostream>

#include <WangMauna/WangMauna.h>
#include <MWL/MWL.h>
#include <MOGL/MOGL.h>
#include <MUI/MUI.h>

DEFINE_LOGGER();
IMPL_LOGGER("Mui Sandbox");

void OnWindowResize(const mwl::WindowResizeEvent& e)
{
	mui::Input& input = mui::GetInput();
	input.Viewport = mui::Vec2f(e.Width, e.Height);

	glViewport(0, 0, e.Width, e.Height);
}

void OnScroll(const mwl::MouseScrollEvent& e)
{
	mui::Input& input = mui::GetInput();

	input.Scroll.Y = e.Delta;
}

void OnMouseMove(const mwl::MouseMovedEvent& e)
{
	mui::Input& input = mui::GetInput();

	input.MousePosition.X = e.X;
	input.MousePosition.Y = e.Y;
}

void OnButtonDown(const mwl::MouseButtonDownEvent& e)
{
	mui::Input& input = mui::GetInput();
	input.MouseLeftDown = true;
}

void OnButtonUp(const mwl::MouseButtonUpEvent& e)
{
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

	mwl::Window* window = mwl::Create(windowProps);
	 
	mwl::CursorData cursor("res/oxy-bluecurve/oxy-bluecurve.inf");
	window->LoadCursorData(cursor);

	window->SetWindowResizeCallback(OnWindowResize);
	window->SetMouseMovedCallback(OnMouseMove);
	window->SetMouseButtonDownCallback(OnButtonDown);
	window->SetMouseButtonUpCallback(OnButtonUp);
	window->SetMouseScrollCalback(OnScroll);

	mogl::OpenGLLoader::LoadOpenGL(mogl::OpenGLVersion::OPENGL_4_6);
	mui::InitForWin32(window->GetNative());
	mui::Input& input = mui::GetInput();
	input.Viewport = { (float)window->GetWidth(), (float)window->GetHeight() };

	int clickMeCount = 0;

	std::string lorem = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. "
		"Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. "
		"Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. "
		"Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. "
		"In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. "
		"Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. "
		"Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. "
		"Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. "
		"Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. "
		"Quisque rutrum. Aenean imperdiet.";

	int loremFlags = mui::TextFlags_None;

	bool showTextWindow = false;
	bool showDragWindow = false;
	bool showSliderWindow = false;
	
	int dragInt1 = 0;
	int dragInt50 = 0;
	int dragInt2[2] = { 0, 0 };
	int dragInt3[3] = { 0, 0, 0 };
	int dragInt4[4] = { 0, 0, 0, 0 };

	float dragFloat1 = 0.0f;
	float dragFloat50 = 0.0f;
	float dragFloatStep = 0.0f;
	float dragFloat2[2] = { 0.0f, 0.0f };
	float dragFloat3[3] = { 0.0f, 0.0f, 0.0f };
	float dragFloat4[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	int sliderInt1 = 0;
	int sliderInt10 = 0;
	int sliderInt2[2] = { 0, 0 };
	int sliderInt3[3] = { 0, 0, 0 };
	int sliderInt4[4] = { 0, 0, 0, 0 };

	float sliderFloat1 = 0.0f;
	float sliderFloat50 = 0.0f;
	float sliderFloatStep = 0.0f;
	float sliderFloat2[2] = { 0.0f, 0.0f };
	float sliderFloat3[3] = { 0.0f, 0.0f, 0.0f };
	float sliderFloat4[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	float clearColor[3] = { 0.0f, 0.2f, 0.3f };

	int count = 0;
	float value = 0.2f;

	while (!window->Closed())
	{
		window->Update();
		window->MakeContextCurrent();

		glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mui::NewFrame();

		mui::Begin("Hello world");

		if (mui::Button("Click me"))
			count++;
		mui::Text("Count: " + std::to_string(count));
		mui::SliderFloat("Slider", &value, 0.1f, 0.0f, 1.0f);
		mui::End();

		mui::Begin("Demo window");

		if (mui::Button("Click me"))
			clickMeCount++;

		mui::Text("Count: " + std::to_string(clickMeCount));
		mui::Separator();

		mui::Checkbox("Show text window", &showTextWindow);
		mui::Checkbox("Show drag window", &showDragWindow);
		mui::Checkbox("Show slider window", &showSliderWindow);

		mui::End();

		{
			mui::Begin("Text window", &showTextWindow);

			mui::Text("Text flags:");
			mui::BeginRadio(&loremFlags);
			mui::Radio("mui::TextFlags_None", mui::TextFlags_None);
			mui::Radio("mui::TextFlags_Clamp", mui::TextFlags_Clamp);
			mui::Radio("mui::TextFlags_Wrap", mui::TextFlags_Wrap);
			mui::Radio("mui::TextFlags_WordWrap", mui::TextFlags_WordWrap);
			mui::EndRadio();

			mui::Separator();
			mui::Text(lorem, loremFlags);

			mui::End();
		}

		{
			mui::Begin("Drag window", &showDragWindow);

			mui::DragInt("Drag int", &dragInt1);
			mui::DragInt("Drag int -50 .. 50", &dragInt50, 1, -50, 50);
			mui::DragInt2("Drag int 2", dragInt2);
			mui::DragInt3("Drag int 3", dragInt3);
			mui::DragInt4("Drag int 4", dragInt4);

			mui::Separator();

			mui::DragFloat("Drag float", &dragFloat1);
			mui::DragFloat("Drag float -50.0 .. 50.0", &dragFloat50, 1, -50, 50);
			mui::DragFloat("Drag float step 0.001", &dragFloatStep, 0.001);
			mui::DragFloat2("Drag float 2", dragFloat2);
			mui::DragFloat3("Drag float 3", dragFloat3);
			mui::DragFloat4("Drag float 4", dragFloat4);

			mui::End();
		}
		
		{
			mui::Begin("Slider window", &showSliderWindow);

			mui::SliderInt("Slider int", &sliderInt1, -10000, 10000);
			mui::SliderInt("Slider int -10 .. 10", &sliderInt10, -10, 10);
			mui::SliderInt2("Slider int 2", sliderInt2, 0, 100);
			mui::SliderInt3("Slider int 3", sliderInt3, 0, 100);
			mui::SliderInt4("Slider int 4", sliderInt4, 0, 100);

			mui::Separator();

			mui::SliderFloat("Slider float", &sliderFloat1, 1.0f, -100.0f, 100.0f);
			mui::SliderFloat("Slider float -50.0 .. 50.0", &sliderFloat50, 1, -50, 50);
			mui::SliderFloat("Slider float step 0.001", &sliderFloatStep, 0.001, 0, 100);
			mui::SliderFloat2("Slider float 2", sliderFloat2, 0.2f, 20, 200);
			mui::SliderFloat3("Slider float 3", sliderFloat3, 0.2f, 20, 200);
			mui::SliderFloat4("Slider float 4", sliderFloat4, 0.2f, 20, 200);

			mui::Separator();

			mui::SliderFloat3("Clear Color", clearColor, 0.01, 0, 1);

			mui::End();
		}
		

		mui::EndFrame();
		mui::Render();

		window->SwapBuffers();
	}

	mui::Shutdown();

	return 0;
}