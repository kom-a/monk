#include <iostream>
#include <functional>
#include <string>

#include "Monk.h"
#include "utils/FileManager.h"

#include <windows.h>

#define BIND_FUNCTION(fn) std::bind(&fn, this, std::placeholders::_1)

using namespace monk;
using namespace monk::gfx;

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
	m_Window->SwapInterval(1);

	if (!utils::OpenGLLoader::LoadOpenGL(utils::OpenGLVersion::OPENGL_3_3))
		DIE("Failed to load OpenGL functions");
	else
	{
		LOG_INFO("OpenGL: ");
		LOG_INFO("	vendor: {0}", glGetString(GL_VENDOR));
		LOG_INFO("	renderer: {0}", glGetString(GL_RENDERER));
		LOG_INFO("	version: {0}", glGetString(GL_VERSION));
	}

	glClearColor(0.4f, 0.6f, 0.8f, 1.0f);
}

Application::~Application()
{
	delete m_Window;
}

void Application::Run()
{
	Renderer2D renderer2D;

	while (!m_Window->Closed())
	{
		if (Input::IsKeyPressed(Key::Escape))
			m_Window->Close();

		glClear(GL_COLOR_BUFFER_BIT);

		renderer2D.Begin(math::Ortho(0.0f, (float)m_Window->GetWidth(), (float)m_Window->GetHeight(), 0.0f, -1.0f, 1.0f));

		renderer2D.DrawRect(math::vec2(100, 100), math::vec2(250, 12), math::vec4(0xff / 355.0f, 0x80 / 355.0f, 0.0f, 1.0f));
		renderer2D.DrawRect(math::vec2(100, 112), math::vec2(250, 123), math::vec4(255 / 255.0f, 215 / 255.0f, 0.0f, 1.0f));

		renderer2D.End();

		m_Window->Update();
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
