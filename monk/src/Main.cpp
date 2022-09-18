#include <iostream>
#include <functional>
#include <string>
#include <chrono>

#include "Monk.h"
#include "utils/FileManager.h"
#include "core/Assert.h"

#include <windows.h>

#define BIND_FUNCTION(fn) std::bind(&fn, this, std::placeholders::_1)


using namespace monk;

class Application
{
public:
	Application();
	~Application();

	void Run();

private:
	void OnEvent(Event& e);
	bool OnWindowResize(WindowResizeEvent& e);

private:
	Window* m_Window;
};

Application::Application()
{
	m_Window = new Window(1280, 720, "Monk");
	m_Window->SetEventCallback(BIND_FUNCTION(Application::OnEvent));
	m_Window->SwapInterval(0);

	if (!utils::OpenGLLoader::LoadOpenGL(utils::OpenGLVersion::OPENGL_3_3))
		DIE("Failed to load OpenGL functions");
	else
	{
		LOG_INFO("OpenGL: ");
		LOG_INFO("	vendor: {0}", glGetString(GL_VENDOR));
		LOG_INFO("	renderer: {0}", glGetString(GL_RENDERER));
		LOG_INFO("	version: {0}", glGetString(GL_VERSION));
	}

	glClearColor(0.45f, 0.1f, 0.8f, 1.0f);
}

Application::~Application()
{
	delete m_Window;
}

void Application::Run()
{
	Gui::Init();

	auto lastTime = std::chrono::system_clock::now();

	bool open1 = true;
	bool open2 = true;
	bool open3 = true;

	while (!m_Window->Closed())
	{
		auto currentTime = std::chrono::system_clock::now();
		auto deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count();
		lastTime = currentTime;

		float delta = deltaTime / 1000.0f;

		LOG_TRACE("FPS: {0}, deltaTime: {1}ms", 1000.0f / delta, delta);

		glClear(GL_COLOR_BUFFER_BIT);

		m_Window->PollEvents();

		if (Input::IsKeyPressed(Key::Escape))
			m_Window->Close();

		if (Input::IsKeyDown(Key::D1))
			open1 = true;
		if (Input::IsKeyDown(Key::D2))
			open2 = true;
		if (Input::IsKeyDown(Key::D3))
			open3 = true;
		
		Gui::NewFrame(math::Ortho(0.0f, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), 0.0f, -1.0f, 1.0f));

		if (open1)
		{
			Gui::Begin("Window1", &open1);
			Gui::End();
		}
		
		if (open2)
		{
			Gui::Begin("Window2", &open2);
			Gui::End();
		}
		
		Gui::EndFrame();

		m_Window->Update();
		Input::Update();
	}
}

void Application::OnEvent(Event& e)
{
	e.Dispatch<WindowResizeEvent>(BIND_FUNCTION(Application::OnWindowResize));
}

bool Application::OnWindowResize(WindowResizeEvent& e)
{
	glViewport(0, 0, e.GetWidth(), e.GetHeight());
	
	return true;
}

int main()
{
	Application app;
	app.Run();

	return 0;
}
