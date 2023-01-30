#include <iostream>
#include <functional>
#include <string>
#include <chrono>

#include "Monk.h"

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
	Renderer2D* m_Renderer;
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

	m_Renderer = new Renderer2D();
	m_Renderer->SetClearColor(math::vec4(0.3f, 0.4f, 0.8f, 1.0f));
}

Application::~Application()
{
	delete m_Window;
	delete m_Renderer;
}

void Application::Run()
{
	Time timer;

	math::vec2 grid = { 128, 128 };
	int fps = 0;
	float lastFpsTime = 0;

	while (!m_Window->Closed())
	{
		float deltaTime = timer.Delta();
		fps++;
		if (timer.Elapsed() - lastFpsTime > 1)
		{
			lastFpsTime = timer.Elapsed();
			LOG_INFO("FPS: {0}", fps);
			fps = 0;
		}

		m_Renderer->Begin(math::Ortho(0, m_Window->GetWidth(), m_Window->GetHeight(), 0, -1, 1));
		m_Renderer->Clear();

		math::vec2 cell = { (float)m_Window->GetWidth() / grid.x, (float)m_Window->GetHeight() / grid.y };
		for (int y = 0; y < grid.y; y++)
		{
			for (int x = 0; x < grid.x; x++)
			{
				m_Renderer->FillRect(math::vec2(x * cell.x, y * cell.y), cell, Random::Color());
			}
		}

		m_Renderer->End();

		m_Window->PollEvents();

		if (Input::IsKeyPressed(Key::Escape))
			m_Window->Close();

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
