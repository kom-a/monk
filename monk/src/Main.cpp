#include <iostream>
#include <functional>
#include <string>
#include <chrono>

#include <MWL/MWL.h>
#include <MOGL/MOGL.h>
#include <MML/MML.h>
#include <MUI/MUI.h>

#include "core/Memory.h"
#include "core/Log.h"
#include "core/Random.h"
#include "graphics/Renderer2D.h"
#include "core/Time.h"

#include "input/Input.h"

#define BIND_FUNCTION(fn) std::bind(&fn, this, std::placeholders::_1)

using namespace monk;

class Application
{
public:
	Application();
	~Application() = default;

	void Run();

private:
	void InitWindow();
	void LoadOpenGL();
	void Update(float deltaTime);

	void OnWindowResize(mwl::WindowResizeEvent e);
	void OnMouseMove(mwl::MouseMovedEvent e);
	void OnButtonDown(mwl::MouseButtonDownEvent& e);
	void OnButtonUp(mwl::MouseButtonUpEvent& e);
	void OnKeyDown(mwl::KeyDownEvent& e);

private:
	monk::Unique<mwl::Window> m_Window;
	monk::Unique<monk::Renderer2D> m_Renderer2D;

	bool m_ShowGUI = true;
};

Application::Application()
{
	InitWindow();
	LoadOpenGL();

	m_Renderer2D = monk::CreateUnique<Renderer2D>();

	mui::InitForWin32((HWND)m_Window->GetNative());
}

void Application::Run()
{
	glClearColor(0.6f, 0.8f, 1.0f, 1.0f);

	Ref<monk::OrthographicCamera> camera = monk::CreateRef<monk::OrthographicCamera>(0, 1, 1, 0, 0, 1);

	monk::Time timer;

	bool open = true;

	while (!m_Window->Closed())
	{
		m_Window->Update();

		glClear(GL_COLOR_BUFFER_BIT);

		m_Renderer2D->Begin(camera);

		float k = 10.0f;

		float step = 1.0f / k;

		for (float i = 0; i < 1; i += step)
		{
			for (float j = 0; j < 1; j += step)
			{
				m_Renderer2D->DrawQuad(mml::vec2(i, j), mml::vec2(step), mml::vec4(i, j, std::sin(timer.Elapsed()) * 0.5f + 0.5f, 1.0f));
			}
		}

		m_Renderer2D->End();

		mui::NewFrame();

		if (m_ShowGUI)
		{

			mui::Begin("Hello world", &open);
			mui::End();

			mui::Begin("Hello");
			mui::End();

			mui::Begin("world");
			mui::End();
		}

		mui::EndFrame();
		mui::Render();

		m_Window->SwapBuffers();
		

		Input::Update();
	}
}

void Application::InitWindow()
{
	auto windowProps = mwl::WindowProps();
	windowProps.Title = L"Monk";
	windowProps.VSync = false;
	windowProps.Width = 800;
	windowProps.Height = 450;
	windowProps.OpenGLContextVersion = mwl::OpenGLVersion::OPENGL_4_6;

	m_Window.reset(mwl::Create(windowProps));

	m_Window->SetWindowResizeCallback(BIND_FUNCTION(Application::OnWindowResize));
	m_Window->SetMouseMovedCallback(BIND_FUNCTION(Application::OnMouseMove));
	m_Window->SetMouseButtonDownCallback(BIND_FUNCTION(Application::OnButtonDown));
	m_Window->SetMouseButtonUpCallback(BIND_FUNCTION(Application::OnButtonUp));
	m_Window->SetKeyDownCallback(BIND_FUNCTION(Application::OnKeyDown));

	m_Window->SetCursor(mwl::Cursor(L"res/oxy-bluecurve/oxy-bluecurve.inf"));
}

void Application::LoadOpenGL()
{
	if (mogl::OpenGLLoader::LoadOpenGL(mogl::OpenGLVersion::OPENGL_4_6) != mogl::OpenGLVersion::OPENGL_4_6)
		LOG_ERROR("Failed to load opengl");
	else
		LOG_INFO("\nOpenGL: \n\tvendor: {0}\n\trenderer: {1}\n\tversion: {2}", glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION));
}

void Application::OnWindowResize(mwl::WindowResizeEvent e)
{
	LOG_INFO("{0}, {1}", e.Width, e.Height);
	glViewport(0, 0, e.Width, e.Height);
}

void Application::OnMouseMove(mwl::MouseMovedEvent e)
{
	mui::GetInput().MousePosition.X = e.X;
	mui::GetInput().MousePosition.Y = e.Y;
}

void Application::OnButtonDown(mwl::MouseButtonDownEvent& e)
{
	mui::GetInput().MouseLeftDown = true;
}

void Application::OnButtonUp(mwl::MouseButtonUpEvent& e)
{
	mui::GetInput().MouseLeftDown = false;
}

void Application::OnKeyDown(mwl::KeyDownEvent& e)
{
	if(e.Key == mwl::KeyCode::Space)
		m_ShowGUI = !m_ShowGUI;
	if (e.Key == mwl::KeyCode::F11)
		m_Window->SetFullscreen(!m_Window->IsFullscreen());
}

int main()
{
	Application app;
	app.Run();

	return 0;
}
